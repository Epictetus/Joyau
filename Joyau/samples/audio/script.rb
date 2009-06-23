initLib
initGfx
initAudio

music = Stream.new
music.setSound("diredocks.bgm")
music.setChannel(1)
music.play

sound = Sound.new
sound.setSound("injury.wav")
sound.setChannel(2)

while mayPlay

  readKeys
  if $keys["cross"]
    sound.play
  end

  startDraw
  drawText(0, 131, "This is an audio sample. It should play a music.")
  drawText(0, 141, "Press cross if you want to hear a sound.")
  endDraw

  sync
  audioSync
end

stopAudio
stopGfx
stopLib
