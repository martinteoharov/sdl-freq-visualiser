asd=0
for i in *; do if [[ $i == *.cpp ]] || [[ $i == *.h ]]; then asd2=$(wc $i | awk '{print $1}'); asd=$(perl -e "print $asd+$asd2"); fi; done; echo $asd;  
