## 1. socket()

## 2. bind()

## 3. listen()

## 4. accept()

## 5. connect()

## 6. send()

## 7. recv()

## 8. TCP 3-way handshake

## 9. TCP byte stream

## 10. Partial send/recv

## 11. Blocking sockets

## 12. Non-blocking sockets

## 13. epoll

## 14. Multiple clients

## 15. TCP vs UDP


------------------------------------------------------------------------------------------------------------------

## TCP 3-Way Handshake and Data Transfer Between Client - Server

 - Setup: Server is listening on 127.0.0.1:8080. Client connects to the Server and sends "hello\n" to server, and server sends the same message
       back to the client.

 - connect() triggers the TCP three-way handshake.

---------------------------------------------------------
sudo tcpdump -i lo -nn -S 'tcp port 8080'
---------------------------------------------------------            
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on lo, link-type EN10MB (Ethernet), snapshot length 262144 bytes

10:17:07.833819 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [S], seq 3813067815, length 0
10:17:07.833865 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [S.], seq 1854576707, ack 3813067816, length 0
10:17:07.833905 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [.], ack 1854576708, length 0

10:17:07.834073 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [P.], seq 3813067816:3813067822, ack 1854576708, length 6: HTTP
10:17:07.834106 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [.], ack 3813067822, length 0
10:17:07.834319 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [P.], seq 1854576708:1854576714, ack 3813067822, length 6: HTTP
10:17:07.834349 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [.], ack 1854576714, length 0

10:17:07.834501 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [F.], seq 3813067822, ack 1854576714, length 0
10:17:07.834660 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [F.], seq 1854576714, ack 3813067823, length 0
10:17:07.834694 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [.], ack 1854576715, length 0

## 3-way handshake

Client->Server [S]  (SYN)       -> to initiate the connection (seq = x)
Server->Client [S.] (SYN + ACK) -> acknowleges and respond (ack = x+1, seq = y)
Client->Server [.]  (ACK)       -> acknowledge (ack = y+1)

[P.] (PSH + ACK) - data transmission
[F.] (FIN + ACK) - gracefully close connection
[R]  (RST - reset/abort connection)

Sequence number: Number of the first byte in a TCP segment. 
Acknowledgment number: Next byte the receiver expects to receive.