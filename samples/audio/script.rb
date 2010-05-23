Joyau.initLib
Joyau.initGfx
Joyau::OslMusic.init

music = Joyau::OslMusic.new("music.mp3", Joyau::OslMusic::FMT_STREAM)
sound = Joyau::OslMusic.new("injury.wav")

music.play(0)

while Joyau.mayPlay
  Joyau::Pad.update

  sound.play(1) if Joyau::Pad.held? Joyau::Pad::CROSS
  
  Joyau.draw do
    Joyau.drawText(0, 0, "This is an audio sample. It should play a music.")
    Joyau.drawText(0, 10, "Press cross if you want to hear a sound.")
  end
end

Joyau::OslMusic.stop
Joyau.stopGfx
Joyau.stopLib
