# CocoTouch

> CocoTouch 可编程互动声音模块
> [详情](http://cocotouch.cn) 购买需添加

## 如何使用源代码

点击 [CocoTouch SDK 及依赖库](http://baidu.com) 下载相关文件，解压之后放到 Arduino IDE 的 Libraries 文件夹下。（暂未发布到 Arduino 官方 SDK 管理库中）

* Windows: <Arduino 安装目录>\Arduino\libraries （默认: C:\Program Files\Arduino\libraries 32位系统）
* Mac: ~/Documents/Arduino/libraries

解压之后可以通过，打开 Arduino ，点击「文件」>「示例」> 「CocoTouch」>「piano」加载示例。接着，USB 连接 CocoTouch 模块，选择对应的串口号，点击「上传」。

![]()

## 制作类课程

* [吉他制作](https://shimo.im/doc/OKPamtLaHwE4DjzO?r=238DY8/%E3%80%8CCocoTouch%20%E4%B9%90%E5%99%A8%E8%AE%BE%E8%AE%A1%E4%B8%8E%E6%99%BA...%E5%90%89%E4%BB%96%E3%80%8D)

## 代码示例

项目内置多个例子，点击 [CocoTouch 代码示例](https://github.com/kber/cocotouch/tree/master/examples) 了解。

* 钢琴 piano.ino
* 打击乐器 jazzdrums.ino
* 编曲 music_compose.ino
* 吃豆人 pacman.ino [「吃豆人游戏」](https://120.77.208.153/examples/pacman/)，触控 0 ~ 3 四个触控分辨为左上右下。

持续更新中...

## API 说明

### <cocotouch.h>

代码文件引用头文件，并且在使用前创建示例。

```cpp
#include <cocotouch.h>

CocoTouch cocotouch;
```

### begin()

默认初始化 CocoTouch 功能（触控，MIDI，蓝牙），若使用到音频需单独初始化。同时也提供 `beginAudio`，`beginMIDI`，`beginTouchSensor`，`beginBLE` API 来单独初始化，其中 `Audio` 和 `MIDI` 不能同时开启，但可以在程序内使用前用 `setAudioMode` 切换。

```cpp
setup() {
  if(!cocotouch.begin()) {
    while(1) {
      ;
    }
  };
}
```

### setAudioMode(AUDIO_MODE_MIDI | AUDIO_MODE_FILE)

切换 `Audio` 和 `MIDI` 模式。

```cpp
setup() {
  // ... begin code 
  cocotouch.setAudioMode(AUDIO_MODE_FILE); // 开启读音频文件模式
}
```

### playAudioFile(filename)

在开启音频文件模式下，输入文件名播放文件，文件只能识别在 SD 卡根目录下。如果想顺序播放所有文件可以调用 `playAudioFiles`。（两个方法均为同步方法）

```cpp
setup() {
  // ... begin code
  cocotouch.setAudioMode(AUDIO_MODE_FILE); // 开启读音频文件模式
  cocotouch.playAudioFile("1.mp3");
}
```

### setMIDIProgram(program, channel)

在指定的通道 `channel` （范围：1 ~ 15）中设定 MIDI 的音色，目前支持所有 GM2 包含的乐器音色，共 128 种，可以访问 (GM2 音色列表)[https://baike.baidu.com/item/gm/498867] 访问详情。

```cpp
setup() {
  // ...begin code
  cocotouch.setMIDIProgram(1, 1);
}
```

想使用 GM2 中定义的打击乐器，使用通道 `channel` 为 0。

### playMIDINote(note, velocity, channel)

在指定的通道 `channel` 中播放音符 `note`，以大钢琴音色为例，可用的 MIDI 音符范围为 21 ~ 108，分别映射了标准 88 键钢琴低音到高音的按键，其中 60 为中央 C 的 DO 的发音。

```cpp
setup() {
  // ...begin code
  cocotouch.setMIDIProgram(0, 1); // 选择大钢琴的音色
  cocotouch.playMIDINote(60, 127, 1); // 播放 Middle C 的 DO 的声音
}

如果是使用 GM2 中的打击乐调用方式有些不同，注意：(GM2 音色列表)[https://baike.baidu.com/item/gm/498867] 查询到的 MIDI 数字对应的是音符数值，是在 `playMIDINote` 中使用。

```cpp
setup() {
  // ...begin code
  cocotouch.playMIDINote(36, 127, 0); // 0 为默认打击乐器
}
```

有些音符在调用 `playMIDINote` 之后声音是比较长的，可以调用 `stopMIDINote` 立即停止。

```cpp
setup() {
  // ...begin code
  cocotouch.setMIDIProgram(41, 1); // 选择小提琴的音色
  cocotouch.playMIDINote(60, 127, 1);
  delay(100);
  cocotouch.stopMIDINote(60, 127, 1); // 等待 100 毫秒之后立即停止
}
```

### getTouched(index)

判断索引为 `index` 的触控点是否已经按下（直接触碰的情况下）。返回值为 1 表示为按下，返回为 0 表示未按下。如果是在 Arduino `loop` 方法中来检测触控点的按下状态，每次 `loop` 被触发的时候都会检测按下的条件是否满足，所以如果在不重复发音，可以使用引入一个状态的变量。

```cpp
uint8_t firstTouched;
setup() { // setup settings }

loop() {
  if (!firstTouched && cocotouch.getTouched(0)) {
    // 在第一个触控点按下的时候
  } 
  firstTouched = cocotouch.getTouched(0);

  if (cocotouch.getTouched(1) && cocotouch.getTouched(2)) {
    // 在第2个和第三个触控点按下的时候
  }
}
```

### getTouchPinValue(index) 和 getTouchPinBaseValue(index)

`getTouchPinValue` 获取索引为 `index` 的触控点的感应值，触碰状态和非触碰状态下这个数值会发生变化，主要用于做更敏感的感应控制。而 `getTouchPinBaseValue` 是用于获取判断触碰时候的基线值（参考值），一般情况下如果 `pinValue` 小于 `baseValue` 时，表示为触碰状态。


### setVolume(vol)

设置 CocoTouch 音频输出的音量，范围是 0 ~ 255。

### sendMessage(message)

通过低功耗蓝牙串口发送消息，默认波特率是 9600。如果消息是静态的，为了节省内存开销，调用 `sendFlashMessage(f(message))` 来发送。

```cpp
setup() {
  // ... begin logic
  cocotouch.sendMessage("hello world");
  cocotouch.sendFlashMessage(f("hello world hello world hello world hello world !!!"));
}
```

## 开源许可

> BSD 3-Clause License
> 
> Copyright (c) 2018, cocoet
> All rights reserved.
> 
> Redistribution and use in source and binary forms, with or without
> modification, are permitted provided that the following conditions are met:
> 
> * Redistributions of source code must retain the above copyright notice, this
>   list of conditions and the following disclaimer.
> 
> * Redistributions in binary form must reproduce the above copyright notice,
>   this list of conditions and the following disclaimer in the documentation
>   and/or other materials provided with the distribution.
> 
> * Neither the name of the copyright holder nor the names of its
>   contributors may be used to endorse or promote products derived from
>   this software without specific prior written permission.
> 
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
> AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
> IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
> DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
> FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
> DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
> SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
> CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
> OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
> OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
