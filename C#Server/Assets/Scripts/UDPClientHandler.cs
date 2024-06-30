using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class UdpClientHandler : MonoBehaviour
{
    private UdpClient udpClient;
    private Thread receiveThread; // ������ ������ ����.
    [Header("Server information")]
    [SerializeField] private bool isServerRunning = false;
    [SerializeField] private int listenPort = 9000; // Unity�� ������ ��Ʈ
    [SerializeField] private int serverPort = 9001; // C++ ������ ������ ��Ʈ
    [SerializeField] private string serverIP = "127.0.0.1"; // ���� IP �ּ�
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
                // ������ �����Ͱ� �ִ��� Ȯ���Ѵ�.
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
        isServerRunning = false; // �÷��׸� �����Ͽ� ���� ������ �����մϴ�.
        udpClient.Close(); //  UdpClient�� �ݾ� ��Ʈ��ũ ���ҽ��� �����մϴ�.
        receiveThread.Abort(); // ���� �����带 ������ �����մϴ�.
    }
}