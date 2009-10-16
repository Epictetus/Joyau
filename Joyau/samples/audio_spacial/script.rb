require 'joyau/old'

initLib
initGfx
initAudio

pos = Vector3f.new(0.0, 0.0, 0.0)
vel = Vector3f.new(0.0, 0.0, 0.1)

Listener.pos = Vector3f.new(0.0, 0.0, 0.0)
Listener.setOrientation(0.0, 0.0, -1.0,  0.0, 1.0, 0.0)

music = Stream.new("music.ogg")
music.pos = pos
music.velocity = vel
skip = false

oldTime = timestamp

while mayPlay and music.update and music.play
  if oldTime <= timestamp - 1
    pos += vel
    music.pos = pos
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
