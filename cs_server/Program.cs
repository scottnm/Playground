using System;

namespace cs_server
{
    internal class NetworkStreamBuffer
    {
        public
        string
        ReadFromClient(
            System.Net.Sockets.TcpClient client)
        {
            int readSize = client.Available;
            if (_bufferSize < readSize)
            {
                _buffer = new byte[readSize];
                _bufferSize = readSize;
            }

            client.GetStream().Read(_buffer, 0, readSize);
            _dataReadSize = readSize;
            return System.Text.Encoding.UTF8.GetString(_buffer, 0, readSize);
        }

        private int _bufferSize = 0;
        private int _dataReadSize = 0;
        private byte[] _buffer = null;
    }

    internal class Program
    {
        static
        string
        GetAddressString(
            System.Net.Sockets.TcpListener tcpListener)
        {
            var ipEndpoint = tcpListener.LocalEndpoint as System.Net.IPEndPoint;
            return String.Format("{0}:{1}", ipEndpoint.Address, ipEndpoint.Port);
        }

        static
        string
        GetAddressString(
            System.Net.Sockets.TcpClient tcpClient)
        {
            var ipEndpoint = tcpClient.Client.RemoteEndPoint as System.Net.IPEndPoint;
            return String.Format("{0}:{1}", ipEndpoint.Address, ipEndpoint.Port);
        }

        static
        void
        Main(
            string[] args)
        {
            var serverTcpSocket = new System.Net.Sockets.TcpListener(System.Net.IPAddress.Parse("127.0.0.1"), 80);

            serverTcpSocket.Start();
            Console.WriteLine("Server has started on {0}! Waiting for connection...", GetAddressString(serverTcpSocket));

            var networkStreamBuffer = new NetworkStreamBuffer();
            while (true)
            {
                Console.WriteLine("Waiting for connection...");
                System.Net.Sockets.TcpClient client = serverTcpSocket.AcceptTcpClient();
                Console.WriteLine("A client connected! {0}", GetAddressString(client));

                System.Net.Sockets.NetworkStream clientStream = client.GetStream();

                int blockCount = 0;
                Console.Write("Waiting for stream data...");
                while (client.Available < 3)
                {
                    System.Threading.Thread.Sleep(100); // poll for new data every 100ms
                    blockCount += 1;
                    if (blockCount % 10 == 0)
                    {
                        Console.Write(".");
                    }
                }
                Console.WriteLine("");

                string request = networkStreamBuffer.ReadFromClient(client);

                Console.WriteLine("Read request: {0}", request);
                if (!request.StartsWith("GET"))
                {
                    Console.WriteLine("Not a GET request. Rejecting client.");
                    continue;
                }

                const string CRLF = "\r\n"; // HTTP/1.1 uses CRLF as the end-of-line marker

                string secWebSocketKey = new System.Text.RegularExpressions
                    .Regex(@"Sec-WebSocket-Key:\s+(\S+)")
                    .Match(request)
                    .Groups[1]
                    .Value
                    .Trim();
                string rawSecWebSocketAcceptString = secWebSocketKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
                byte[] rawSecWebSocketAcceptBytes = System.Text.Encoding.UTF8.GetBytes(rawSecWebSocketAcceptString);
                byte[] hashedSecWebSocketAcceptBytes = System.Security.Cryptography.SHA1.Create().ComputeHash(rawSecWebSocketAcceptBytes);

                string[] responseLines = new string[] {
                    "HTTP/1.1 101 Switching Protocols",
                    "Connection: Upgrade",
                    "Upgrade: websocket",
                    "Sec-WebSocket-Accept: " + Convert.ToBase64String(hashedSecWebSocketAcceptBytes),
                    "",
                };

                string response = String.Join(CRLF, responseLines);
                Console.WriteLine("Writing response: {0}", response);

                byte[] responseBytes = System.Text.Encoding.UTF8.GetBytes(response);
                clientStream.Write(responseBytes);
            }
        }
    }
}
