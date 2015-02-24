package com.phildogames.fourgenerals;

import java.util.*;
import java.net.*;

import android.app.*;
import android.content.*;
import android.view.*;
import android.widget.*;
import android.os.*;
import android.util.Log;
import android.graphics.*;
import android.media.*;
import android.hardware.*;
import android.net.*;
import android.net.wifi.*;

import org.libsdl.app.SDLActivity;

public class FourGeneralsActivity extends SDLActivity
{
  private static Context context;

  public static native void setupEnv();
  public static String getIP()
  {
    ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
    NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
    if(activeNetworkInfo.getType() == ConnectivityManager.TYPE_WIFI)
    {
      WifiManager myWifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
      WifiInfo myWifiInfo = myWifiManager.getConnectionInfo();
      int ipAddress = myWifiInfo.getIpAddress();

      return android.text.format.Formatter.formatIpAddress(ipAddress);
    }
    else if(activeNetworkInfo.getType() == ConnectivityManager.TYPE_MOBILE)
    {
      try
      {
        Enumeration<NetworkInterface> interfs = NetworkInterface.getNetworkInterfaces();
        NetworkInterface activeInterf = null;
        String hostName = NetworkInterface.getByInetAddress(InetAddress.getLocalHost()).getName();
        InetAddress ret = null;

        while(interfs.hasMoreElements())
        {
          try
          {
            activeInterf = interfs.nextElement();
            if(activeInterf.isLoopback()) { continue; }
            else if(!hostName.equalsIgnoreCase(activeInterf.getDisplayName()))
            {
              ret = activeInterf.getInetAddresses().nextElement();
              break;
            }
          }
          catch(NoSuchElementException e) { continue; }
        }

        return ret.getHostAddress();
      }
      catch (Exception e) { }
    }
    else { return null; }

    return null;
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setupEnv();
    context = getApplicationContext();
    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
  }
}

