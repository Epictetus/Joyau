require 'joyaugem'

require 'joyau/init'

Joyau.init(:intrafont)

#begin
  require 'spec'
=begin
rescue LoadError
  require 'rubygems/dependency_installer'

  inst = Gem::DependencyInstaller.new
  inst.install("rspec").each do |gem|
    puts "Installed #{gem.name}!"
    puts "Please run this program again."
  end

  Joyau.exitGame
end
=end

Dir["spec/*.rb"].each do |f| 
  puts "loading #{f}..."
  require(f)
end

Spec::Runner.run

Joyau.exitGame
Joyau.stop
