# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/xenial64"
  config.disksize.size = "20GB"
  config.vm.provider "virtualbox" do |vb|
    # Customize the amount of memory on the VM:
    vb.memory = "2048"
  end

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y libncurses5-dev ruby{,-dev} libmagick{core-dev,wand-dev} imagemagick linux-image-generic
    apt-get build-dep -y linux-image-$(uname -r)
    apt-get install -y build-essential linux-headers-$(uname -r)
    gem install vimdeck
    reboot
  SHELL
end
