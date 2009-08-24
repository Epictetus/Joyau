initLib
initGfx
initAudio

music = Stream.new("music.ogg")
sound = Sound.new("injury.wav")

while mayPlay and music.update

  readKeys
  if $keys["cross"]
    sound.play
  end

  startDraw
  drawText(0, 131, "This is an audio sample. It should play a music.")
  drawText(0, 141, "Press cross if you want to hear a sound.")
  endDraw

  if !music.play
    break
  end

  sync
end

stopAudio
stopGfx
stopLib
