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
  public static native void setupEnv();
  public static void broadcast(String str)
  {
    Log.v("FG", "Broadcasting: "+str);
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setupEnv();
  }

  private class SocketServerThread extends Thread
  {
    int port = 8080;
    int count = 0;
    ServerSocket serverSocket;

    @override
    public void run()
    {
      try
      {
        /*
        //If you want to do shit on main thread, this is how:
        MainActivity.this.runOnUiThread(new Runnable()
        {
          @Override
          public void run()
          {
            //Do shit
          }
        });
        */

        serverSocket = new ServerSocket(port);
        serverSocket.getLocalPort(); //NN: I think just returns port?

        while(true)
        {
          Socket socket = serverSocket.accept();
          count++;
          socket.getInetAddress(); //NN: I think returns IP Address?
          socket.getPort(); //NN: I think returns port?

          SocketServerReplyThread socketServerReplyThread = new SocketServerReplyThread(socket, count);
          socketServerReplyThread.run();
        }
      }
      catch(IOException e) { e.printStackTrace(); }

    }
  }

  private class SocketServerReplyThread extends Thread
  {
    private Socket hostThreadSocket;
    int count;

    SocketServerReplyThread(Socket socket, int c)
    {
      hostThreadSocket = socket;
      count = c;
    }

    @Override
    public void run()
    {
      OutputStream outputStream;

      try
      {
        outputStream = hostThreadSocket.getOutputStream();
        PrintStream printStream = new PrintStream(outputStream);
        printStream.print("Whatup");
        printStream.close();
      }
      catch(IOException e) { e.printStackTrace(); }

    }
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

/*
//http://android-er.blogspot.com/2014/02/android-sercerclient-example-server.html
private class SocketServerThread extends Thread
{
  static final int SocketServerPORT = 8080;
  int count = 0;

  @Override
  public void run()
  {
    try
    {
      serverSocket = new ServerSocket(SocketServerPORT);
      MainActivity.this.runOnUiThread(new Runnable()
        {
          @Override
          public void run()
          {
            info.setText("I'm waiting here: " + serverSocket.getLocalPort());
          }
        });

      while (true)
      {
        Socket socket = serverSocket.accept();
        count++;
        message += "#" + count + " from " + socket.getInetAddress() + ":" + socket.getPort() + "\n";

        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              msg.setText(message);
            }
          });

        SocketServerReplyThread socketServerReplyThread = new SocketServerReplyThread(socket, count);
        socketServerReplyThread.run();

      }
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
  }
}

private class SocketServerReplyThread extends Thread
{
  private Socket hostThreadSocket;
  int cnt;

  SocketServerReplyThread(Socket socket, int c)
  {
    hostThreadSocket = socket;
    cnt = c;
  }

  @Override
  public void run()
  {
    OutputStream outputStream;
    String msgReply = "Hello from Android, you are #" + cnt;

    try
    {
      outputStream = hostThreadSocket.getOutputStream();
      PrintStream printStream = new PrintStream(outputStream);
      printStream.print(msgReply);
      printStream.close();

      message += "replayed: " + msgReply + "\n";

      MainActivity.this.runOnUiThread(new Runnable()
        {
          @Override
          public void run()
          {
            msg.setText(message);
          }
        });
    }
    catch(IOException e)
    {
      e.printStackTrace();
      message += "Something wrong! " + e.toString() + "\n";
    }

    MainActivity.this.runOnUiThread(new Runnable()
      {
        @Override
        public void run()
        {
          msg.setText(message);
        }
      });
  }
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
          ip += "SiteLocalAddress: " + inetAddress.getHostAddress() + "\n";
        }
      }
    }
  }
  catch(SocketException e)
  {
    e.printStackTrace();
    ip += "Something Wrong! " + e.toString() + "\n";
  }

  return ip;
}
*/




//http://android-er.blogspot.com/2014/08/bi-directional-communication-between.html
//Server
public class MainActivity extends Activity
{
  TextView info, infoip, msg;
  String message = "";
  ServerSocket serverSocket;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    info = (TextView) findViewById(R.id.info);
    infoip = (TextView) findViewById(R.id.infoip);
    msg = (TextView) findViewById(R.id.msg);

    infoip.setText(getIpAddress());

    Thread socketServerThread = new Thread(new SocketServerThread());
    socketServerThread.start();
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();

    if (serverSocket != null)
    {
      try                  { serverSocket.close(); }
      catch(IOException e) { e.printStackTrace(); }
    }
  }

  private class SocketServerThread extends Thread
  {
    static final int SocketServerPORT = 8080;
    int count = 0;

    @Override
    public void run()
    {
      Socket socket = null;
      DataInputStream dataInputStream = null;
      DataOutputStream dataOutputStream = null;

      try
      {
        serverSocket = new ServerSocket(SocketServerPORT);
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              info.setText("I'm waiting here: " + serverSocket.getLocalPort());
            }
          });

        while(true)
        {
          socket = serverSocket.accept();
          dataInputStream = new DataInputStream(socket.getInputStream());
          dataOutputStream = new DataOutputStream(socket.getOutputStream());

          String messageFromClient = "";

          //If no message sent from client, this code will block the program
          messageFromClient = dataInputStream.readUTF();

          count++;
          message += "#" + count + " from " + socket.getInetAddress() + ":" + socket.getPort() + "\n" +
            "Msg from client: " + messageFromClient + "\n";

          MainActivity.this.runOnUiThread(new Runnable()
            {
              @Override
              public void run()
              {
                msg.setText(message);
              }
            });

          String msgReply = "Hello from Android, you are #" + count;
          dataOutputStream.writeUTF(msgReply);
        }
      }
      catch (IOException e)
      {
        e.printStackTrace();
        final String errMsg = e.toString();
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              msg.setText(errMsg);
            }
          });
      }
      finally
      {
        if(socket != null)
        {
          try                  { socket.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        if(dataInputStream != null)
        {
          try                  { dataInputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        if(dataOutputStream != null)
        {
          try                  { dataOutputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }
      }
    }

  }

  private String getIpAddress()
  {
    String ip = "";
    try
    {
      Enumeration<NetworkInterface> enumNetworkInterfaces = NetworkInterface
      .getNetworkInterfaces();
      while (enumNetworkInterfaces.hasMoreElements())
      {
        NetworkInterface networkInterface = enumNetworkInterfaces.nextElement();
        Enumeration<InetAddress> enumInetAddress = networkInterface.getInetAddresses();
        while(enumInetAddress.hasMoreElements())
        {
          InetAddress inetAddress = enumInetAddress.nextElement();

          if(inetAddress.isSiteLocalAddress())
          {
            ip += "SiteLocalAddress: " + inetAddress.getHostAddress() + "\n";
          }
        }
      }
    }
    catch (SocketException e)
    {
      e.printStackTrace();
      ip += "Something Wrong! " + e.toString() + "\n";
    }

    return ip;
  }
}


//http://android-er.blogspot.com/2014/08/bi-directional-communication-between.html
//Client
public class MainActivity extends Activity
{
  EditText editTextAddress, editTextPort;
  Button buttonConnect, buttonClear;

  EditText welcomeMsg;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    editTextAddress = (EditText) findViewById(R.id.address);
    editTextPort = (EditText) findViewById(R.id.port);
    buttonConnect = (Button) findViewById(R.id.connect);
    buttonClear = (Button) findViewById(R.id.clear);

    welcomeMsg = (EditText)findViewById(R.id.welcomemsg);

    buttonConnect.setOnClickListener(buttonConnectOnClickListener);
  }

  OnClickListener buttonConnectOnClickListener = new OnClickListener()
  {
    @Override
    public void onClick(View arg0)
    {
      String tMsg = welcomeMsg.getText().toString();
      if(tMsg.equals(""))
      {
        tMsg = null;
        Toast.makeText(MainActivity.this, "No Welcome Msg sent", Toast.LENGTH_SHORT).show();
      }

      MyClientTask myClientTask = new MyClientTask(editTextAddress.getText().toString(), Integer.parseInt(editTextPort.getText().toString()), tMsg);
      myClientTask.execute();
    }
  };

  public class MyClientTask extends AsyncTask<Void, Void, Void>
  {
    String dstAddress;
    int dstPort;
    String response = "";
    String msgToServer;

    MyClientTask(String addr, int port, String msgTo)
    {
      dstAddress = addr;
      dstPort = port;
      msgToServer = msgTo;
    }

    @Override
    protected Void doInBackground(Void... arg0)
    {
      Socket socket = null;
      DataOutputStream dataOutputStream = null;
      DataInputStream dataInputStream = null;

      try
      {
        socket = new Socket(dstAddress, dstPort);
        dataOutputStream = new DataOutputStream(socket.getOutputStream());
        dataInputStream = new DataInputStream(socket.getInputStream());

        if(msgToServer != null)
        {
          dataOutputStream.writeUTF(msgToServer);
        }
        response = dataInputStream.readUTF();
      }
      catch (UnknownHostException e)
      {
        e.printStackTrace();
        response = "UnknownHostException: " + e.toString();
      } catch (IOException e)
      {
        e.printStackTrace();
        response = "IOException: " + e.toString();
      }
      finally
      {
        if (socket != null)
        {
          try                  { socket.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        if (dataOutputStream != null)
        {
          try                  { dataOutputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        if (dataInputStream != null)
        {
          try                  { dataInputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }
      }
      return null;
    }

    @Override
    protected void onPostExecute(Void result)
    {
      //looks like 'response' is text?
      super.onPostExecute(result);
    }
  }
}


//http://android-er.blogspot.com/2014/08/implement-simple-android-chat.html
//Server
public class MainActivity extends ActionBarActivity
{
  static final int SocketServerPORT = 8080;
  TextView infoIp, infoPort, chatMsg;
  String msgLog = "";
  List<ChatClient> userList;
  ServerSocket serverSocket;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    infoIp = (TextView) findViewById(R.id.infoip);
    infoPort = (TextView) findViewById(R.id.infoport);
    chatMsg = (TextView) findViewById(R.id.chatmsg);

    infoIp.setText(getIpAddress());

    userList = new ArrayList<ChatClient>();

    ChatServerThread chatServerThread = new ChatServerThread();
    chatServerThread.start();
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();

    if(serverSocket != null)
    {
      try { serverSocket.close(); }
      catch(IOException e) { e.printStackTrace(); }
    }
  }

  private class ChatServerThread extends Thread
  {
    @Override
    public void run()
    {
      Socket socket = null;

      try
      {
        serverSocket = new ServerSocket(SocketServerPORT);
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              infoPort.setText("I'm waiting here: " + serverSocket.getLocalPort());
            }
          });

        while(true)
        {
          socket = serverSocket.accept();
          ChatClient client = new ChatClient();
          userList.add(client);
          ConnectThread connectThread = new ConnectThread(client, socket);
          connectThread.start();
        }
      }
      catch(IOException e)
      {
        e.printStackTrace();
      }
      finally
      {
        if(socket != null)
        {
          try                  { socket.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }
      }
    }
  }

  private class ConnectThread extends Thread
  {
    Socket socket;
    ChatClient connectClient;
    String msgToSend = "";

    ConnectThread(ChatClient client, Socket socket)
    {
      connectClient = client;
      this.socket= socket;
      client.socket = socket;
      client.chatThread = this;
    }

    @Override
    public void run()
    {
      DataInputStream dataInputStream = null;
      DataOutputStream dataOutputStream = null;

      try
      {
        dataInputStream = new DataInputStream(socket.getInputStream());
        dataOutputStream = new DataOutputStream(socket.getOutputStream());

        String n = dataInputStream.readUTF();

        connectClient.name = n;

        msgLog += connectClient.name + " connected@" + connectClient.socket.getInetAddress() + ":" + connectClient.socket.getPort() + "\n";
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              chatMsg.setText(msgLog);
            }
          });

        dataOutputStream.writeUTF("Welcome " + n + "\n");
        dataOutputStream.flush();

        broadcastMsg(n + " join our chat.\n");

        while(true)
        {
          if(dataInputStream.available() > 0)
          {
            String newMsg = dataInputStream.readUTF();

            msgLog += n + ": " + newMsg;
            MainActivity.this.runOnUiThread(new Runnable()
              {
                @Override
                public void run()
                {
                  chatMsg.setText(msgLog);
                }
              });

            broadcastMsg(n + ": " + newMsg);
          }

          if(!msgToSend.equals(""))
          {
            dataOutputStream.writeUTF(msgToSend);
            dataOutputStream.flush();
            msgToSend = "";
          }
        }
      }
      catch(IOException e)
      {
        e.printStackTrace();
      }
      finally
      {
        if(dataInputStream != null)
        {
          try                  { dataInputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }
        if(dataOutputStream != null)
        {
          try                  { dataOutputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        userList.remove(connectClient);
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              Toast.makeText(MainActivity.this, connectClient.name + " removed.", Toast.LENGTH_LONG).show();

              msgLog += "-- " + connectClient.name + " leaved\n";
              MainActivity.this.runOnUiThread(new Runnable()
                {
                  @Override
                  public void run()
                  {
                    chatMsg.setText(msgLog);
                  }
                });

              broadcastMsg("-- " + connectClient.name + " leaved\n");
            }
          });
      }
    }

    private void sendMsg(String msg)
    {
      msgToSend = msg;
    }
  }

  private void broadcastMsg(String msg)
  {
    for(int i=0; i<userList.size(); i++)
    {
      userList.get(i).chatThread.sendMsg(msg);
      msgLog += "- send to " + userList.get(i).name + "\n";
    }

    MainActivity.this.runOnUiThread(new Runnable()
      {
        @Override
        public void run()
        {
          chatMsg.setText(msgLog);
        }
      });
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
            ip += "SiteLocalAddress: " + inetAddress.getHostAddress() + "\n";
          }
        }
      }
    }
    catch(SocketException e)
    {
      e.printStackTrace();
      ip += "Something Wrong! " + e.toString() + "\n";
    }
    return ip;
  }

  class ChatClient
  {
    String name;
    Socket socket;
    ConnectThread chatThread;
  }
}











//http://android-er.blogspot.com/2014/08/simple-android-chat-application-client.html
//Client
public class MainActivity extends ActionBarActivity
{
  static final int SocketServerPORT = 8080;

  LinearLayout loginPanel, chatPanel;

  EditText editTextUserName, editTextAddress;
  Button buttonConnect;
  TextView chatMsg, textPort;

  EditText editTextSay;
  Button buttonSend;
  Button buttonDisconnect;

  String msgLog = "";

  ChatClientThread chatClientThread = null;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    loginPanel = (LinearLayout)findViewById(R.id.loginpanel);
    chatPanel = (LinearLayout)findViewById(R.id.chatpanel);

    editTextUserName = (EditText) findViewById(R.id.username);
    editTextAddress = (EditText) findViewById(R.id.address);
    textPort = (TextView) findViewById(R.id.port);
    textPort.setText("port: " + SocketServerPORT);
    buttonConnect = (Button) findViewById(R.id.connect);
    buttonDisconnect = (Button) findViewById(R.id.disconnect);
    chatMsg = (TextView) findViewById(R.id.chatmsg);

    buttonConnect.setOnClickListener(buttonConnectOnClickListener);
    buttonDisconnect.setOnClickListener(buttonDisconnectOnClickListener);

    editTextSay = (EditText)findViewById(R.id.say);
    buttonSend = (Button)findViewById(R.id.send);

    buttonSend.setOnClickListener(buttonSendOnClickListener);
  }

  OnClickListener buttonDisconnectOnClickListener = new OnClickListener()
  {
    @Override
    public void onClick(View v)
    {
      if(chatClientThread==null) { return; }
      chatClientThread.disconnect();
    }
  };

  OnClickListener buttonSendOnClickListener = new OnClickListener()
  {
    @Override
    public void onClick(View v)
    {
      if(editTextSay.getText().toString().equals("")) { return; }
      if(chatClientThread==null)                      { return; }

      chatClientThread.sendMsg(editTextSay.getText().toString() + "\n");
    }
  };

  OnClickListener buttonConnectOnClickListener = new OnClickListener()
  {
    @Override
    public void onClick(View v)
    {
      String textUserName = editTextUserName.getText().toString();
      if(textUserName.equals(""))
      {
        Toast.makeText(MainActivity.this, "Enter User Name", Toast.LENGTH_LONG).show();
        return;
      }

      String textAddress = editTextAddress.getText().toString();
      if (textAddress.equals(""))
      {
        Toast.makeText(MainActivity.this, "Enter Addresse", Toast.LENGTH_LONG).show();
        return;
      }

      msgLog = "";
      chatMsg.setText(msgLog);
      loginPanel.setVisibility(View.GONE);
      chatPanel.setVisibility(View.VISIBLE);

      chatClientThread = new ChatClientThread(textUserName, textAddress, SocketServerPORT);
      chatClientThread.start();
    }
  };

  private class ChatClientThread extends Thread
  {
    String name;
    String dstAddress;
    int dstPort;

    String msgToSend = "";
    boolean goOut = false;

    ChatClientThread(String name, String address, int port)
    {
      this.name = name;
      dstAddress = address;
      dstPort = port;
    }

    @Override
    public void run()
    {
      Socket socket = null;
      DataOutputStream dataOutputStream = null;
      DataInputStream dataInputStream = null;

      try
      {
        socket = new Socket(dstAddress, dstPort);
        dataOutputStream = new DataOutputStream(socket.getOutputStream());
        dataInputStream = new DataInputStream(socket.getInputStream());
        dataOutputStream.writeUTF(name);
        dataOutputStream.flush();

        while(!goOut)
        {
          if(dataInputStream.available() > 0)
          {
            msgLog += dataInputStream.readUTF();

            MainActivity.this.runOnUiThread(new Runnable()
              {
                @Override
                public void run()
                {
                  chatMsg.setText(msgLog);
                }
              });
          }

          if(!msgToSend.equals(""))
          {
            dataOutputStream.writeUTF(msgToSend);
            dataOutputStream.flush();
            msgToSend = "";
          }
        }

      }
      catch(UnknownHostException e)
      {
        e.printStackTrace();
        final String eString = e.toString();
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              Toast.makeText(MainActivity.this, eString, Toast.LENGTH_LONG).show();
            }
          });
      }
      catch(IOException e)
      {
        e.printStackTrace();
        final String eString = e.toString();
        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              Toast.makeText(MainActivity.this, eString, Toast.LENGTH_LONG).show();
            }
          });
      }
      finally
      {
        if(socket != null)
        {
          try                  { socket.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        if (dataOutputStream != null)
        {
          try                  { dataOutputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        if (dataInputStream != null)
        {
          try                  { dataInputStream.close(); }
          catch(IOException e) { e.printStackTrace(); }
        }

        MainActivity.this.runOnUiThread(new Runnable()
          {
            @Override
            public void run()
            {
              loginPanel.setVisibility(View.VISIBLE);
              chatPanel.setVisibility(View.GONE);
            }
          });
      }
    }

    private void sendMsg(String msg)
    {
      msgToSend = msg;
    }

    private void disconnect()
    {
      goOut = true;
    }
  }
}

