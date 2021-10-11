using System;
using System.Net;
using System.Net.Sockets;

namespace cs_server
{
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
        void
        Main(
            string[] args)
        {
            var serverTcpSocket = new System.Net.Sockets.TcpListener(System.Net.IPAddress.Parse("127.0.0.1"), 80);

            serverTcpSocket.Start();
            Console.WriteLine("Server has started on {0}! Waiting for connection...", GetAddressString(serverTcpSocket));

            System.Net.Sockets.TcpClient client = serverTcpSocket.AcceptTcpClient();
            Console.WriteLine("A client connected!");

            System.Net.Sockets.NetworkStream clientStream = client.GetStream();

            while (true)
            {
                int blockCount = 0;
                Console.Write("Waiting for stream data...");
                while (!clientStream.DataAvailable)
                {
                    System.Threading.Thread.Sleep(100); // poll for new data every 100ms
                    blockCount += 1;
                    if (blockCount % 10 == 0)
                    {
                        Console.Write(".");
                    }
                }
                Console.WriteLine("");

                Byte[] readBytes = new Byte[client.Available];
                clientStream.Read(readBytes, 0, readBytes.Length);
                Console.WriteLine("Read bytes: {0}", System.Text.Encoding.UTF8.GetString(readBytes, 0, readBytes.Length));
            }
        }
    }
}
