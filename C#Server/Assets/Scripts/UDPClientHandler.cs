using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class UdpClientHandler : MonoBehaviour
{
    private UdpClient udpClient;
    private Thread receiveThread;
    [Header("Server information")]
    [SerializeField] private bool isServerRunning = false;
    [SerializeField] private int listenPort = 9000; // Unity가 수신할 포트
    [SerializeField] private int serverPort = 9001; // C++ 서버가 수신할 포트

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
        while (isServerRunning)
        {
            try
            {
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

    void Update()
    {
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
        isServerRunning = false;
        udpClient.Close();
        receiveThread.Abort();
    }
}