class Color
  attr_reader :hash

  def r
    @hash["r"]
  end

  def g
    @hash["g"]
  end

  def b
    @hash["b"]
  end
  
  def a
    @hash["a"]
  end

  def r=(val)
    @hash["r"] = val
  end

  def g=(val)
    @hash["g"] = val
  end

  def b=(val)
    @hash["b"] = val
  end
  
  def a=(val)
    @hash["a"] = val
  end

  def ==(other)
    @hash["r"] == other.r and
      @hash["g"] == other.g and
      @hash["b"] == other.b and
      @hash["a"] == other.a
  end

  def initialize(*args)
    @hash = Hash.new

    if args.size == 1
      @hash = args[0]
    elsif args.size >= 3
      @hash["r"] = args[0]
      @hash["g"] = args[1]
      @hash["b"] = args[2]
      if args.size > 3
        @hash["a"] = args[3]
      else
        @hash["a"] = 255
      end
    else
      @hash["r"] = 255
      @hash["g"] = 255
      @hash["b"] = 255
      @hash["a"] = 255
    end
  end
end
