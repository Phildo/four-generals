package com.phildogames.fourgenerals;

import android.app.*;
import android.content.*;
import android.view.*;
import android.widget.AbsoluteLayout;
import android.os.*;
import android.util.Log;
import android.graphics.*;
import android.media.*;
import android.hardware.*;

import java.io.*;
import java.net.*;
import java.util.*;

import org.libsdl.app.SDLActivity;

public class FourGeneralsActivity extends SDLActivity
{
  int port = 8080;

  ServerSocket serverSocket;
  ArrayList<Connection> connections;

  ServerThread serverThread;

  public static native void initNativeEnv();
  public static void broadcast(String str)
  {
    Log.v("FG", "Broadcasting: " + str);
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    connections = new ArrayList<Connection>();

    initNativeEnv();
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();
  }

  private void connectAsServer()
  {
    serverThread = new ServerThread();
    serverThread.start();
  }

  private void connectAsClient()
  {
    Connection connection = new Connection();
    try { connection.socket = new Socket("192.168.2.1", 8080); } catch(Exception e){}

    ConnectionThread connectionThread = new ConnectionThread(connection);
    connectionThread.start();
  }

  private void broadcastMsg(String msg)
  {
    for(int i = 0; i < connections.size(); i++)
    {
      connections.get(i).thread.sendMsg(msg);
    }
  }

  private class ServerThread extends Thread
  {
    @Override
    public void run()
    {
      try
      {
        serverSocket = new ServerSocket(port);

        while(true)
        {
          Connection connection = new Connection();
          connection.socket = serverSocket.accept(); //will block here

          ConnectionThread connectionThread = new ConnectionThread(connection);
          connectionThread.start();
        }
      }
      catch(IOException e) { e.printStackTrace(); }
      finally
      {
        if(serverSocket != null) try{ serverSocket.close(); } catch(IOException e) { e.printStackTrace(); }
      }
    }
  }

  private class ConnectionThread extends Thread
  {
    Connection connection;
    String message;
    String response;

    boolean requestsDisconnect = false;

    ConnectionThread(Connection connection)
    {
      this.connection = connection;
      this.connection.thread = this;
    }

    public void sendMsg(String msg) { message = msg; }
    public void disconnect() { requestsDisconnect = true; }

    @Override
    public void run()
    {
      connections.add(connection);

      DataOutputStream dataOutputStream = null;
      DataInputStream dataInputStream = null;

      try
      {
        dataOutputStream = new DataOutputStream(this.connection.socket.getOutputStream());
        dataInputStream = new DataInputStream(this.connection.socket.getInputStream());

        while(!requestsDisconnect)
        {
          if(dataInputStream.available() > 0)
          {
            response = dataInputStream.readUTF();
            Log.v("FG", "Received: " + response);
          }

          if(message != null)
          {
            dataOutputStream.writeUTF(message);
            dataOutputStream.flush();
            Log.v("FG", "Sent: " + message);
            message = null;
          }
        }
      }
      catch(UnknownHostException e) { e.printStackTrace(); }
      catch(IOException e) { e.printStackTrace(); }
      finally
      {
        if(this.connection.socket != null) { try { this.connection.socket.close(); } catch(IOException e) { e.printStackTrace(); } }
        if(dataInputStream        != null) { try{ dataInputStream.close(); }         catch(IOException e) { e.printStackTrace(); } }
        if(dataOutputStream       != null) { try{ dataOutputStream.close(); }        catch(IOException e) { e.printStackTrace(); } }
        connections.remove(connection);
      }
    }
  }

  class Connection
  {
    Socket socket;
    ConnectionThread thread;
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

