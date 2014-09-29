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
  static int port = 8080;

  static ServerSocket serverSocket;
  static ArrayList<Connection> connections;

  static ServerThread serverThread;

  public static native void initNativeEnv();
  public static void broadcast(String str)
  {
    broadcastMsg(str);
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    connections = new ArrayList<Connection>();

    initNativeEnv();

    Log.v("FG", "IP Address: " + getIpAddress());
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();
  }

  static private void connectAsServer()
  {
    Log.v("FG", "Connecting As Server");
    serverThread = new ServerThread();
    serverThread.start();
  }

  static private void connectAsClient()
  {
    Log.v("FG", "Connecting As Client");
    Connection connection = new Connection();
    try { connection.socket = new Socket("192.168.2.7", port); } catch(Exception e){ Log.v("FG","Failed connection"); }

    ConnectionThread connectionThread = new ConnectionThread(connection);
    connectionThread.start();
  }

  static private void broadcastMsg(String msg)
  {
    Log.v("FG", "Broadcasting: " + msg);
    for(int i = 0; i < connections.size(); i++)
    {
      connections.get(i).thread.sendMsg(msg);
    }
  }

  static private class ServerThread extends Thread
  {
    @Override
    public void run()
    {
      try
      {
        Log.v("FG", "Listen as Server");
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
        Log.v("FG", "Stop Server");
        if(serverSocket != null) try{ serverSocket.close(); } catch(IOException e) { e.printStackTrace(); }
      }
    }
  }

  static private class ConnectionThread extends Thread
  {
    Connection connection;
    String messOut;
    String messIn;

    boolean requestsDisconnect = false;

    ConnectionThread(Connection connection)
    {
      this.connection = connection;
      this.connection.thread = this;
    }

    public void sendMsg(String msg) { messOut = msg; }
    public void disconnect() { requestsDisconnect = true; }

    @Override
    public void run()
    {
      connections.add(connection);

      DataOutputStream dataOut = null;
      DataInputStream dataIn = null;

      try
      {
        Log.v("FG", "Connection Made");
        dataOut = new DataOutputStream(this.connection.socket.getOutputStream());
        dataIn = new DataInputStream(this.connection.socket.getInputStream());

        StringBuilder sb = new StringBuilder();
        boolean inReady = false;
        char c;

        while(!requestsDisconnect)
        {
          while(dataIn.available() > 0 && !inReady) //if message is ready jump out and handle it
          {
            c = (char)dataIn.readByte(); //read as byte (ascii) cast to char (utf16)
            if(c == '\n') inReady = true;
            else sb.append(c);
          }

          if(inReady)
          {
            messIn = sb.toString();
            sb = new StringBuilder();
            inReady = false;
            Log.v("FG", "Received: " + messIn);
          }

          if(messOut != null)
          {
            dataOut.writeUTF(messOut);
            dataOut.flush();
            Log.v("FG", "Sent: " + messOut);
            messOut = null;
          }
        }
      }
      catch(UnknownHostException e) { e.printStackTrace(); }
      catch(IOException e) { e.printStackTrace(); }
      finally
      {
        Log.v("FG", "Connection Stopped");
        if(this.connection.socket != null) { try { this.connection.socket.close(); } catch(IOException e) { e.printStackTrace(); } }
        if(dataIn                 != null) { try { dataIn.close(); }                 catch(IOException e) { e.printStackTrace(); } }
        if(dataOut                != null) { try { dataOut.close(); }                catch(IOException e) { e.printStackTrace(); } }
        connections.remove(connection);
      }
    }
  }

  static class Connection
  {
    Socket socket;
    ConnectionThread thread;
  }


  static private String getIpAddress()
  {
    String ip = "";
    try
    {
      Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
      while (interfaces.hasMoreElements())
      {
        NetworkInterface iface = interfaces.nextElement();
        //filters out 127.0.0.1 and inactive interfaces
        if (iface.isLoopback() || !iface.isUp()) continue;

        Enumeration<InetAddress> addresses = iface.getInetAddresses();
        while(addresses.hasMoreElements())
        {
          InetAddress addr = addresses.nextElement();
          ip += iface.getDisplayName() + ": " + addr.getHostAddress() + "; ";
        }
      }
    }
    catch (SocketException e)
    {
      return "" + e.getMessage() + e.getStackTrace();//throw new RuntimeException(e);
    }
    return ip;
  }
}

