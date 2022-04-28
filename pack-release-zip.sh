#!/bin/bash

rm -rf data-clean
rm -rf data-maps
rm -f data-clean.zip
rm -f data-maps.zip

mkdir -p data-maps/osu/
cp -r maps data-maps/osu/
cp -r skins data-maps/osu/
cp -r languages data-maps/osu/
cp maps/beatmaps.json data-maps/osu/
cp maps/beatmaps_state.json data-maps/osu/ 
rm data-maps/osu/maps/beatmaps.json 
rm data-maps/osu/maps/beatmaps_state.json

mkdir -p data-clean/osu/
cp -r skins data-clean/osu/
cp -r languages data-clean/osu/

find data-maps -name '.DS_Store' -type f -delete
find data-clean -name '.DS_Store' -type f -delete

cd data-maps
zip -qq -r -X data-maps.zip osu
mv data-maps.zip ../
cd ..

cd data-clean
zip -qq -r -X data-clean.zip osu
mv data-clean.zip ../
cd ..

rm -rf data-clean
rm -rf data-maps

echo "====> DONE! <===="
