initLib
initGfx
initAudio

music = Stream.new
music.loadOgg("music.ogg")

sound = Sound.new
sound.loadWav("injury.wav")

while mayPlay

  readKeys
  if $keys["cross"]
    sound.play
  end

  startDraw
  drawText(0, 131, "This is an audio sample. It should play a music.")
  drawText(0, 141, "Press cross if you want to hear a sound.")
  endDraw

  music.play
  music.update

  sync
end

stopAudio
stopGfx
stopLib
