using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.RegularExpressions;

namespace cs_server
{
    internal class NetworkStreamBuffer
    {
        public string ReadFromClient(
            System.Net.Sockets.TcpClient client)
        {
            ReadToBuffer(client);
            return System.Text.Encoding.UTF8.GetString(_buffer, 0, _dataReadSize);
        }

        public byte[] ReadBytesFromClient(
            System.Net.Sockets.TcpClient client)
        {
            ReadToBuffer(client);
            return (byte[])_buffer.Clone();
        }

        private void ReadToBuffer(
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
        }

        private int _bufferSize = 0;
        private int _dataReadSize = 0;
        private byte[] _buffer = null;
    }

    internal class MyServer
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

        public delegate void WaitForDataCallBack();

        static
        void
        WaitForData(
            System.Net.Sockets.TcpClient client,
            int byteCountToWaitFor,
            WaitForDataCallBack callback)
        {
            System.Net.Sockets.NetworkStream clientStream = client.GetStream();

            int blockCount = 0;
            while (!clientStream.DataAvailable || client.Available < byteCountToWaitFor)
            {
                System.Threading.Thread.Sleep(100); // poll for new data every 100ms
                blockCount += 1;
                if (blockCount % 10 == 0)
                {
                    callback();
                }
            }
        }

        private void WriteLogSection(
            string sectionName,
            bool startOrEnd)
        {
            Console.WriteLine(
                "============ {0:12}: {1} ============{2}",
                sectionName.ToUpper(),
                startOrEnd ? "START" : "END",
                startOrEnd ? "" : "\n");
        }

        public void Run()
        {
            var serverTcpSocket = new System.Net.Sockets.TcpListener(System.Net.IPAddress.Parse("127.0.0.1"), 80);

            serverTcpSocket.Start();
            Console.WriteLine("Server has started on {0}!", GetAddressString(serverTcpSocket));

            var networkStreamBuffer = new NetworkStreamBuffer();

            while (true)
            {
                WriteLogSection("connection", true);
                Console.WriteLine("Waiting for connection...");
                System.Net.Sockets.TcpClient client = serverTcpSocket.AcceptTcpClient();
                Console.WriteLine("A client connected! {0}", GetAddressString(client));
                WriteLogSection("connection", false);

                System.Net.Sockets.NetworkStream clientStream = client.GetStream();

                WriteLogSection("handshake", true);
                Console.Write("Waiting for stream data...");
                WaitForData(client, 3, () => Console.Write("."));
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
                    "",
                };

                string response = String.Join(CRLF, responseLines);
                Console.WriteLine("Writing response: {0}", response);

                byte[] responseBytes = System.Text.Encoding.UTF8.GetBytes(response);
                clientStream.Write(responseBytes, 0, response.Length);
                WriteLogSection("handshake", false);

                WriteLogSection("chatting", true);

                while (true)
                {
                    Console.Write("Waiting for client data...");
                    WaitForData(client, 2, () => Console.Write("."));
                    byte[] bytes = networkStreamBuffer.ReadBytesFromClient(client);
                    {
                        // TODO:
                        // Copy-pasted from mozilla.
                        // Fix after checking (for now I'm getting tired)
                        bool fin = (bytes[0] & 0b10000000) != 0;
                        bool mask = (bytes[1] & 0b10000000) != 0; // must be true, "All messages from the client to the server have this bit set"

                        int opcode = bytes[0] & 0b00001111; // expecting 1 - text message
                        int msglen = bytes[1] - 128; // & 0111 1111
                        int offset = 2;

                        if (msglen < 126)
                        {
                            // do nothing. msglen is the length of the message;
                        }
                        else if (msglen == 126)
                        {
                            // was ToUInt16(bytes, offset) but the result is incorrect
                            msglen = BitConverter.ToUInt16(new byte[] { bytes[3], bytes[2] }, 0);
                            offset = 4;
                        }
                        else if (msglen == 127)
                        {
                            Console.WriteLine("TODO: msglen == 127, needs qword to store msglen");
                            // i don't really know the byte order, please edit this
                            // msglen = BitConverter.ToUInt64(new byte[] { bytes[5], bytes[4], bytes[3], bytes[2], bytes[9], bytes[8], bytes[7], bytes[6] }, 0);
                            // offset = 10;
                        }

                        if (msglen == 0)
                        {
                            Console.WriteLine("msglen == 0");
                        }
                        else if (mask)
                        {
                            byte[] decoded = new byte[msglen];
                            byte[] masks = new byte[4] { bytes[offset], bytes[offset + 1], bytes[offset + 2], bytes[offset + 3] };
                            offset += 4;

                            for (int i = 0; i < msglen; ++i)
                                decoded[i] = (byte)(bytes[offset + i] ^ masks[i % 4]);

                            string text = System.Text.Encoding.UTF8.GetString(decoded);
                            Console.WriteLine("{0}", text);
                            if (text.ToLower() == "quit")
                            {
                                break;
                            }
                        }
                        else
                        {
                            Console.WriteLine("mask bit not set");
                        }

                        Console.WriteLine();
                    }
                }

                WriteLogSection("chatting", false);
                break; // FIXME: how can I break this loop but have the default be to loop
            }
            serverTcpSocket.Stop();
        }
    }

    internal class Program
    {
        public static void Main()
        {
            new MyServer().Run();
            new ReferenceServer().Run();
        }
    }
}
