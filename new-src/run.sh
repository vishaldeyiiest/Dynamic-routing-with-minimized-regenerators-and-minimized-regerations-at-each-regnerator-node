read -p "Enter file name: " file
./min_regnode $file >> ./Result/NSFNET/1
./minreg_at_node $file >> ./Result/NSFNET/2
./min_regntns $file >> ./Result/NSFNET/3

