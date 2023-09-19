#! /bin/bash
echo "copy rules"
sudo cp ./99-nhc.rules /etc/udev/rules.d
echo "Reload rules"
sudo udevadm control  --reload-rules
echo -e "DONE"

