using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class UdpClientHandler : MonoBehaviour
{
    private UdpClient udpClient;
    private Thread receiveThread; // 별도의 쓰레드 생성.
    [Header("Server information")]
    [SerializeField] private bool isServerRunning = false;
    [SerializeField] private int listenPort = 9000; // Unity가 수신할 포트
    [SerializeField] private int serverPort = 9001; // C++ 서버가 수신할 포트
    [SerializeField] private string serverIP = "127.0.0.1"; // 서버 IP 주소
    void Start()
    {
        ConnectedToServer();
    }
    private void ConnectedToServer()
    {
        try
        {
            udpClient = new UdpClient(listenPort);
            receiveThread = new Thread(new ThreadStart(ReceiveData));
            receiveThread.IsBackground = true;
            receiveThread.Start();
            isServerRunning = true;
        }
        catch (Exception ex)
        {
            Debug.LogError("Error initializing UdpClient: " + ex.Message);
            isServerRunning = false;
        }
    }
    private void ReceiveData()
    {
        IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, listenPort);
        while (true == isServerRunning)
        {
            try
            {
                // 수신할 데이터가 있는지 확인한다.
                if (udpClient.Available > 0)
                {
                    byte[] data = udpClient.Receive(ref remoteEndPoint);
                    string message = Encoding.UTF8.GetString(data);
                    Debug.Log("Received message: " + message);
                }
            }
            catch (SocketException ex)
            {
                Debug.LogError("SocketException receiving data: " + ex.Message);
                isServerRunning = false;
                break;
            }
            catch (Exception ex)
            {
                Debug.LogError("Exception receiving data: " + ex.Message);
                isServerRunning = false;
                break;
            }
        }
    }

    public void SendPacket(string message)
    {
        try
        {
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);
            byte[] data = Encoding.UTF8.GetBytes(message);
            udpClient.Send(data, data.Length, serverEndPoint);
            Debug.Log("Sent message: " + message);
        }
        catch (Exception ex)
        {
            Debug.LogError("Exception sending data: " + ex.Message);
        }
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.S))
        {
            SendPacket("Hello from Unity client!");
        }

        if (!isServerRunning)
        {
            Debug.Log("Server is not running. Exiting play mode.");
#if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
#else
            Application.Quit();
#endif
        }
    }

    void OnApplicationQuit()
    {
        isServerRunning = false; // 플래그를 설정하여 수신 루프를 종료합니다.
        udpClient.Close(); //  UdpClient를 닫아 네트워크 리소스를 해제합니다.
        receiveThread.Abort(); // 수신 스레드를 강제로 종료합니다.
    }
}