class Projectil < Sprite  
  def play
    if @count == nil
      @count = 100
    end

    @count -= 1
    if getDirection == $directions["RIGHT"]
      move(3, 0)
    elsif getDirection == $directions["LEFT"]
      move(-3, 0)
    elsif getDirection == $directions["DOWN"]
      move(0, 3)
    elsif getDirection == $directions["UP"]
      move(0, -3)
    end
  end

  def count
    @count
  end
  
  attr_accessor :hasCollided
end
