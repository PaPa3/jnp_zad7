# shellcheck disable=SC2035
rm *bmp
g++ -Wall -Wextra -pedantic -O3 -fconcepts *.cc
./a.out

for f in *.bmp
do
  diff "$f" "../../common/zadanie7/$f"
done