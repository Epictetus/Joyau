require 'joyau/inherited'

class Hero < Joyau::Sprite
  joyau_inherited

  def initialize(pic = nil)
    setPicture("pic.png".dup)
    @projectils = []
  end

  def draw
    super()
    @projectils.each { |pro| pro.draw }
  end
    
  def play(advs)
    @projectils.each do |pro|
      pro.has_collided = false
      pro.play

      advs.each do |adv|
        if adv.collide(pro)
          $score += 1
          pro.has_collided = true
        end
      end
    end

    @projectils.reject! { |pro| pro.count <= 0 || pro.has_collided }

    Joyau::Pad.update
    
    move(0, -2) if Joyau::Pad.held? Joyau::Pad::UP
    move(0, 2)  if Joyau::Pad.held? Joyau::Pad::DOWN
    move(2, 0)  if Joyau::Pad.held? Joyau::Pad::RIGHT
    move(-2, 0) if Joyau::Pad.held? Joyau::Pad::LEFT
    
    if Joyau::Pad.held? Joyau::Pad::CROSS
      self.alpha -= 2 if self.alpha - 2 > 20
    elsif Joyau::Pad.held? Joyau::Pad::TRIANGLE
      self.alpha += 2 if self.alpha + 2 < 256
    end
    
    if Joyau::Pad.pressed? Joyau::Pad::SQUARE
      if @projectils.length < 2
        pro = Projectil.new("proj.png")
        pro.alpha = 127 if self.alpha <= 127
        pro.pos = Joyau::Point.new(x, y)
        pro.direction = direction
        
        @projectils << pro
      end
    end
  end
end
