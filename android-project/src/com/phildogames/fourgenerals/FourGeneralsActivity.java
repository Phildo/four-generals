package com.phildogames.fourgenerals;

import android.app.*;
import android.content.*;
import android.view.*;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.os.*;
import android.util.Log;
import android.graphics.*;
import android.media.*;
import android.hardware.*;

import java.io.*;
import java.net.*;
import java.util.Enumeration;

import org.libsdl.app.SDLActivity;

public class FourGeneralsActivity extends SDLActivity
{
  int port = 8080;

  //server
  List<Client> userList;
  ServerSocket serverSocket;

  //client
  ClientThread clientThread;

  public static native void initNativeEnv();
  public static void broadcast(String str)
  {
    Log.v("FG", "Broadcasting: "+str);
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    userList = new ArrayList<Client>();

    //init server
    ServerThread serverThread = new ServerThread();
    serverThread.start();

    initNativeEnv();
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();

    if(serverSocket)
    {
      try                  { serverSocket.close(); }
      catch(IOException e) { e.printStackTrace(); }
    }
  }

  private void connect()
  {
    //init client
    clientThread = new ClientThread("User","192.168.2.1",8080);
    clientThread.start();
  }

  private void disconnect()
  {
    if(clientThread==null) { return; }
    clientThread.disconnect();
  }

  private class ClientThread extends Thread
  {
    String address;
    int port;
    String message;
    String response;

    boolean requestsDisconnect = false;

    ClientThread(String address, int port)
    {
      this.address = address;
      this.port = port;
    }

    public void sendMsg(String msg) { message = msg; }
    public void disconnect() { requestsDisconnect = true; }

    @Override
    public void run()
    {
      Socket socket = null;
      DataOutputStream dataOutputStream;
      DataInputStream dataInputStream;

      try
      {
        socket = new Socket(address, port);
        dataOutputStream = new DataOutputStream(socket.getOutputStream());
        dataInputStream = new DataInputStream(socket.getInputStream());

        while(!requestsDisconnect)
        {
          if(dataInputStream.available() > 0)
          {
            response = dataInputStream.readUTF();
          }

          if(message)
          {
            dataOutputStream.writeUTF(message);
            dataOutputStream.flush();
            message = null;
          }
        }
      }
      catch(UnknownHostException e) { e.printStackTrace(); }
      catch(IOException e) { e.printStackTrace(); }
      finally
      {
        if(socket)           { try { socket.close(); }           catch(IOException e) { e.printStackTrace(); } }
        if(dataOutputStream) { try { dataOutputStream.close(); } catch(IOException e) { e.printStackTrace(); } }
        if(dataInputStream)  { try { dataInputStream.close(); }  catch(IOException e) { e.printStackTrace(); } }
      }
    }
  }

  private class ServerThread extends Thread
  {
    @Override
    public void run()
    {
      Socket socket = null;

      try
      {
        serverSocket = new ServerSocket(port);

        while(true)
        {
          socket = serverSocket.accept();

          Client client = new Client();
          userList.add(client);

          ServerClientThread serverClientThread = new ServerClientThread(client, socket);
          serverClientThread.start();
        }
      }
      catch(IOException e) { e.printStackTrace(); }
      finally
      {
        if(socket) { try{ socket.close(); } catch(IOException e) { e.printStackTrace(); } }
      }
    }
  }

  private class ServerClientThread extends Thread
  {
    Socket socket;
    Client client;
    String message;
    String response;

    ServerClientThread(Client client, Socket socket)
    {
      this.client = client;
      this.socket = socket;

      client.socket = socket;
      client.thread = this;
    }

    public void sendMsg(String msg) { message = msg; }

    @Override
    public void run()
    {
      DataInputStream dataInputStream = null;
      DataOutputStream dataOutputStream = null;

      try
      {
        dataInputStream = new DataInputStream(socket.getInputStream());
        dataOutputStream = new DataOutputStream(socket.getOutputStream());

        while(true)
        {
          if(dataInputStream.available() > 0)
          {
            response = dataInputStream.readUTF();
            broadcastMsg(newMsg);
          }

          if(message)
          {
            dataOutputStream.writeUTF(message);
            dataOutputStream.flush();
            message = null;
          }
        }
      }
      catch(IOException e) { e.printStackTrace(); }
      finally
      {
        if(dataInputStream)  { try{ dataInputStream.close(); }  catch(IOException e) { e.printStackTrace(); } }
        if(dataOutputStream) { try{ dataOutputStream.close(); } catch(IOException e) { e.printStackTrace(); } }
        userList.remove(client);
      }
    }
  }

  private void broadcastMsg(String msg)
  {
    for(int i = 0; i < userList.size(); i++)
    {
      userList.get(i).thread.sendMsg(msg);
    }
  }

  class Client
  {
    Socket socket;
    ServerClientThread thread;
  }

  private String getIpAddress()
  {
    String ip = "";
    try
    {
      Enumeration<NetworkInterface> enumNetworkInterfaces = NetworkInterface.getNetworkInterfaces();
      while(enumNetworkInterfaces.hasMoreElements())
      {
        NetworkInterface networkInterface = enumNetworkInterfaces.nextElement();
        Enumeration<InetAddress> enumInetAddress = networkInterface.getInetAddresses();
        while(enumInetAddress.hasMoreElements())
        {
          InetAddress inetAddress = enumInetAddress.nextElement();
          if(inetAddress.isSiteLocalAddress())
          {
            ip += "SiteLocalAddress: "+inetAddress.getHostAddress()+"\n";
          }
        }
      }
    }
    catch(SocketException e) { e.printStackTrace(); }

    return ip;
  }
}

