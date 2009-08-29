class Projectil < Sprite  
  def play
    if @count == nil
      @count = 100
    end

    @count -= 1
    if direction == Sprite::RIGHT
      move(3, 0)
    elsif direction == Sprite::LEFT
      move(-3, 0)
    elsif direction == Sprite::DOWN
      move(0, 3)
    elsif direction == Sprite::UP
      move(0, -3)
    end
  end

  def count
    @count
  end
  
  attr_accessor :hasCollided
end
