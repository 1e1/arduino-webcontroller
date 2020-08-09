#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname $BIN_DIR)

readonly INPUT_HTML="$BIN_DIR/html/_.html"
readonly TEMP_GZ="$BIN_DIR/_.gz"
readonly OUTPUT_H="$BASE_DIR/sketch_WSlave/_webApp.h"

readonly SED_BACKUP_EXT=".sed"


# remove
# ^/s+
# \s+$
# \n+
# replace '/all.txt' => '/$'

echo "original HTML"
ls -al $INPUT_HTML
#sed -e ':a' -e 'N' -e '$!ba' -e 's/\n//g' $INPUT_HTML
gzip -c -9 $INPUT_HTML > $TEMP_GZ

echo "compressed HTML"
ls -al $TEMP_GZ

echo "write .h"
echo "" >  $OUTPUT_H
########################################

cat <<EOT >> $OUTPUT_H
#ifndef webApp_H_
#define webApp_H_

#define WEBPAGE { \\
EOT

hexdump -v -e '/1 "0x%02X, "' $TEMP_GZ >> $OUTPUT_H
sed -i $SED_BACKUP_EXT '$s/, $/ \\/' $OUTPUT_H

cat <<EOT >> $OUTPUT_H
}
#endif // webApp_H_
EOT

########################################
ls -al $OUTPUT_H

rm "$OUTPUT_H$SED_BACKUP_EXT"
rm "$TEMP_GZ"
