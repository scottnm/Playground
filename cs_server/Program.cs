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

            System.Net.Sockets.TcpClient acceptedClient = serverTcpSocket.AcceptTcpClient();
            Console.WriteLine("A client connected!");
        }
    }
}
