package com.phildogames.fourgenerals;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

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

import org.libsdl.app.SDLActivity;

public class FourGeneralsActivity extends SDLActivity
{
  //THIS MAINLY SERVES AS AN EXAMPLE FOR PHIL'S OWN REFERENCE. DON'T ACTUALLY CALL.
  public static native String cFunc(String str);
  public static void jFunc(String str)
  {
    Log.v("FG", "Java Func!:"+str);
  }

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
}

