Joyau.initLib
Joyau.initGfx
Joyau.initAudio

music = Joyau::Stream.new("music.ogg")
sound = Joyau::Sound.new("injury.wav")

while Joyau.mayPlay and music.update and music.play
  Joyau::Pad.update

  sound.play if Joyau::Pad.held? Joyau::Pad::CROSS
  
  Joyau.startDraw
  Joyau.drawText(0, 0, "This is an audio sample. It should play a music.")
  Joyau.drawText(0, 10, "Press cross if you want to hear a sound.")
  Joyau.endDraw

  Joyau.sync
end

Joyau.stopAudio
Joyau.stopGfx
Joyau.stopLib
