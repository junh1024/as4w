Appearance Sounds (like the Platinum sounds in Mac OS8) for Windows (which aren't in windows events sounds), runnable as a exe.

Main window is supposed to be blank since i'm too lazy to make a ＼(　**¯∇¯)ノ ☆ＦＡＢＵＬＯＵＳ UI**

**[Download here](https://code.google.com/p/as4w/issues/detail?id=1)**

Extract all the files into a new folder. With AS4W open, try some UI actions like dragging windows around, switching focus, and choosing menu items. Not all apps emit accessibility events, which is what this app listens for to make these sounds.


For _full effect_ follow [this tutorial](http://www.7tutorials.com/how-customize-sound-scheme-used-your-computer) and link

```
"Open Program" with ApplicationLaunched.wav
"Close Program" with ApplicationQuit.wav
"Menu Command" MenuItemRelease.wav
"Minimize" with WindowCollapseDown.wav
"Restore Up" with WindowCollapseUp.wav
```

You can find these sounds in \extrasounds

You need to do this manually yourself due to limitations in W32 accessibility events


Other limitations:
  * The path length is max. 127. If you like super mega ultra deep folderception, contact me or file an issue.
  * If you're launching the app in wierd ways (ie, the current working directory isn't where the sounds folder is), then you won't hear the intended sounds. Move the sounds folder to the CWD. A launch via double-click or shortcut should work fine.

[Source direct link here](https://code.google.com/p/as4w/source/browse/as4w.c)