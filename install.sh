sudo apt-get -y install libncurses5-dev libncursesw5-dev
make -C src/

if [ $? -ne 0 ]; then
  echo -e "\npseudo top: install failed\n"
fi
