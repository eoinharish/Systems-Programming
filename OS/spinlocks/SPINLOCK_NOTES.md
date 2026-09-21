# Spinlocks: A Quick Reference

## 1. Naive Spinlock (Plain TAS — Test-And-Set)
Every spin iteration performs an **atomic read-modify-write (RMW)**.

```cpp
while (locked.exchange(true)) { /* spin */ }
```

**Problem:** Each `exchange()` call requires exclusive ownership of the cache line, even when just checking. Under contention, this causes constant cache-line invalidation across cores ("cache-line ping-pong") — expensive and wasteful.

---

## 2. TTAS (Test-and-Test-And-Set)
**Idea:** Only spin on a cheap plain **read** once the lock is known to be contended; avoid repeated expensive atomic RMWs while waiting.

There are two equivalent orderings:

**(a) Read-first (textbook form)**
```cpp
while (true) {
    while (locked.load()) { }        // "test" — cheap, cached read
    if (!locked.exchange(true))      // "test-and-set" — only when it looks free
        break;
}
```

**(b) Exchange-first (this implementation)** — try the atomic op immediately, and only fall back to spin-reading if it fails:
```cpp
void lock() {
    while (1) {
        // Try and grab the lock; return if we get it
        if (!locked.exchange(true, std::memory_order_acquire)) {
            return;
        }

        // Didn't get it — spin on a plain load (cached locally),
        // instead of retrying the expensive exchange() every time
        while (locked.load(std::memory_order_relaxed)) { }
        // loop back and try exchange() again
    }
}

void unlock() {
    locked.store(false, std::memory_order_release);
}
```

Form (b) skips the initial read on the very first attempt — in the common **uncontended** case (lock is free), you go straight to `exchange()` and return immediately, saving one memory access compared to form (a). Once the lock *is* contended, both forms behave the same: fall back to cheap `load()` spinning, and only re-attempt the atomic `exchange()` once the read suggests the lock might be free.

**Memory ordering (form (b)):**
- `exchange(true, memory_order_acquire)` — prevents critical-section operations from being reordered *before* the lock is acquired.
- `load(memory_order_relaxed)` while spinning — no ordering needed here; this is just polling a flag, not synchronizing shared data yet.
- `store(false, memory_order_release)` — ensures everything done inside the critical section is visible to the next thread that acquires the lock.

This acquire/release pairing gives full correctness (no data races on the protected data) while avoiding the cost of `seq_cst` on every spin iteration.

**Why it's better than plain TAS either way:** A plain `load()` can be satisfied from the thread's local cached copy (shared state) — no coherence traffic generated while the lock stays held. Traffic only spikes at the moment the lock actually changes.

---

## 3. Spinning Locally
The general principle behind TTAS's benefit: make waiting threads spin on **data that lives in their own local cache**, rather than data that requires exclusive/write access from every core.

- Plain `load()` on a `std::atomic<bool>` → satisfiable locally (as long as no one writes to it).
- `exchange()` / `fetch_add()` etc. → always require the cache line exclusively, defeating local spinning.

This principle extends further in **queue-based locks** (MCS, CLH), where each thread spins on a private, per-thread flag — no shared cache line is touched by multiple spinning threads at all.

---

## 4. Active Backoff (a.k.a. Busy-Wait Backoff)
Still spinning (not yielding the CPU/OS scheduler), but **spacing out retries** instead of hammering continuously.

```cpp
int delay = 1;
while (locked.load()) {
    for (int i = 0; i < delay; i++) { _mm_pause(); }  // CPU "pause" hint
    delay = std::min(delay * 2, MAX_DELAY);            // exponential backoff
}
```

- Reduces contention on the cache-coherence bus without giving up the CPU core.
- `pause` instruction (`_mm_pause()`) hints to the CPU that this is a spin-wait loop — reduces power draw and avoids pipeline flush penalties on some architectures.
- Good middle ground when critical sections are short but contention is real.

---

## 5. Passive Backoff (Yielding / Sleeping)
Instead of continuing to burn CPU, the waiting thread **gives up its timeslice** or sleeps briefly, letting the OS scheduler run other work.

```cpp
while (locked.load()) {
    std::this_thread::yield();          // give up timeslice
    // or: std::this_thread::sleep_for(std::chrono::microseconds(50));
}
```

- Useful when contention is high or critical sections are long — spinning would waste more CPU than it saves.
- This is essentially blending spinlock behavior with mutex-like behavior (many real mutex implementations spin briefly, then fall back to OS blocking).

---

## Summary Table

| Technique | Retries via | Cache traffic | CPU usage while waiting | Best for |
|---|---|---|---|---|
| Naive spinlock (TAS) | atomic RMW every iteration | High | High | Very low contention only |
| TTAS | read first, RMW only when free | Low (until lock frees) | High | Short critical sections, low–medium contention |
| Active backoff | TTAS + increasing pause | Lower | High (but reduced) | Medium contention |
| Passive backoff | yield/sleep instead of spin | Low | Low | High contention or long critical sections |

**Rule of thumb:** naive TAS → TTAS is almost always a free win. Backoff (active or passive) is worth adding once you expect real contention; passive backoff (or a real mutex) wins once spinning would waste more CPU than it saves.

**Read vs. exchange ordering:** whether you check `load()` before `exchange()` (textbook TTAS) or try `exchange()` first and fall back to `load()` on failure, both are TTAS — the defining property is *not retrying the atomic RMW every spin iteration*. Exchange-first shaves one memory access off the uncontended fast path; read-first is more traditional/readable. Either is fine.