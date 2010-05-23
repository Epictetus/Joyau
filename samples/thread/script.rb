Joyau.initLib
Joyau.initGfx

class Manager
  def initialize
    @need_update = false
  end

  def update
    return unless @need_update
    Joyau.sync
    Joyau::Pad.update
    @need_update = false
  end

  def ask_update
    @need_update = true
  end
end

manager = Manager.new

update_thread = Thread.new(manager) { |manager|
  begin
    manager.update
  end while Joyau.mayPlay
}

main_thread = Thread.new(manager) { |manager|
  background = Joyau::Sprite.new("bg.png")
  img        = Joyau::Sprite.new("image.png")

  while Joyau.mayPlay
    img.move(1,  0) if Joyau::Pad.held? Joyau::Pad::RIGHT
    img.move(-1, 0) if Joyau::Pad.held? Joyau::Pad::LEFT
    img.move(0,  1) if Joyau::Pad.held? Joyau::Pad::DOWN
    img.move(0, -1) if Joyau::Pad.held? Joyau::Pad::UP
    
    Joyau.startDraw
    background.draw
    img.draw
    Joyau.endDraw
    
    manager.ask_update
  end
}

update_thread.join
main_thread.join

Joyau.stopGfx
Joyau.stopLib
