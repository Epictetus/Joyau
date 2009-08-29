initLib
initGfx
initAudio

vector =  Vector3f.new(1.0, 1.0, 1.0)
Listener.pos = Vector3f.new(-1.0, -1.0, -1.0)

music = Stream.new("music.ogg")
music.pos = vector
skip = false

oldTime = timestamp

while mayPlay and music.update and music.play
  if oldTime <= timestamp - 3
    vector.x += 1.0
    vector.y += 1.0
    vector.z += 1.0
    music.pos = vector
  end

  if !skip
    startDraw
    drawText(0, 0, "This is an audio sample. It should play a music.")
    endDraw
  end

  skip = sync
end

stopAudio
stopGfx
stopLib
