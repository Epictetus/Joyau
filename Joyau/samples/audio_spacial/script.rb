initLib
initGfx
initAudio

music = Stream.new("music.ogg")
music.pos = Vector3f.new(1.0, 1.0, 1.0)
skip = false

while mayPlay and music.update and music.play
  if !skip
    startDraw
    drawText(0, 131, "This is an audio sample. It should play a music.")
    endDraw
  end

  skip = sync
end

stopAudio
stopGfx
stopLib
