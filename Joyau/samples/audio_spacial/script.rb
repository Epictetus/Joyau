Joyau.initLib
Joyau.initGfx
Joyau.initAudio

pos = Joyau::Vector3f.new(0.0, 0.0, 0.0)
vel = Joyau::Vector3f.new(0.0, 0.0, 0.1)

Joyau::Listener.pos = Joyau::Vector3f.new(0.0, 0.0, 0.0)
Joyau::Listener.setOrientation(0.0, 0.0, -1.0,  0.0, 1.0, 0.0)

music = Joyau::Stream.new("music.ogg")
music.pos = pos
music.velocity = vel
skip = false

oldTime = Joyau.timestamp

while Joyau.mayPlay and music.update and music.play
  if oldTime <= Joyau.timestamp - 1
    pos += vel
    music.pos = pos
  end

  if !skip
    Joyau.startDraw
    Joyau.drawText(0, 0, "This is an audio sample. It should play a music.")
    Joyau.endDraw
  end

  skip = Joyau.sync
end

Joyau.stopAudio
Joyau.stopGfx
Joyau.stopLib
