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
  public static native void cFunc();
  public static void jFunc()
  {
    Log.v("FG", "Java Func!");
  }

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    Log.v("FG", "Java onCreate()");
    super.onCreate(savedInstanceState);
    Log.v("FG", "Java dubalidoo()");
    cFunc();
    Log.v("FG", "Java hey?");
  }
}

