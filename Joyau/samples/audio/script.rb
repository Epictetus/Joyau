initLib
initGfx
initAudio

music = Stream.new("music.ogg")
sound = Sound.new("injury.wav")

while mayPlay and music.update and music.play
  Pad.update

  if Pad.held? Pad::CROSS
    sound.play
  end

  startDraw
  drawText(0, 0, "This is an audio sample. It should play a music.")
  drawText(0, 10, "Press cross if you want to hear a sound.")
  endDraw

  sync
end

stopAudio
stopGfx
stopLib
