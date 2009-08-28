class FunnyDrawable < RubyDrawable
  def draw
    puts "they drawed me !"
  end

  def isOn(argX, argY) # nothing collide ! Nothing is on !
    puts argX.to_s, argY.to_s
    return false
  end
  
  def collide(drawable)
    puts "false"
    return false
  end

  def boundingRect()
    Rect.new(500, 500, 0, 0) # I've a funny bonding rect too !
  end
end

initLib
initGfx

draw = FunnyDrawable.new

sprite = Sprite.new("sprite.png")

draw.resize(10, 10)
draw.setPos(10, 10)

puts sprite.collide(draw).to_s

# They should collide, since corners of the funny drawable are on
# the corners of the sprite.
if !sprite.collide(draw)
  # however... we have redefined boundingRect :)
  # Basically, our (it's your as well as it's mine :) C++ class RubyDrawable
  # has redefined the methods for collide, etc.
  # it calls our Ruby method. That's nice, BUT by default, we'll have an
  # unlimited numbers of recursions. That's why we've redefined everything here.
  draw.draw
end

stopGfx
stopLib
