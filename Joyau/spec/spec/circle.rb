require 'joyau_helper'
require "spec/drawable.rb"

describe Joyau::Circle do
  before :each do
    @drawable = Joyau::Circle.new
    @drawable.center = Joyau::Point.new(1, 1)
    @drawable.radius = 1

    @drawable.clearMove
  end

  it_should_behave_like "all drawables"

  it "should not be located at the center" do
    @drawable.center = Joyau::Point.new(50, 50)
    @drawable.radius = 40

    @drawable.should_not be_at(50, 50)
  end
end
