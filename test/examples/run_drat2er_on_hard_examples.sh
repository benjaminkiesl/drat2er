#!/bin/sh
# This script should be executed in the directory where it is located.
echo "======================================="
echo "Transforming Proof of Urquhart Formula."
echo "======================================="
echo ""
../../build/bin/drat2er Urquhart-s5-b1.shuffled.cnf Urquhart-s5-b1.shuffled.drat -v Urquhart-s5-b1.shuffled.er
echo ""
echo "=========================================="
echo "Transforming Proof of Pigeon Hole Formula."
echo "=========================================="
echo ""
../../build/bin/drat2er hole20.cnf hole20.drat -v hole20.er
echo ""
echo "==================================================="
echo "Transforming Proof of Two-Pigeons-Per-Hole Formula."
echo "==================================================="
echo ""
../../build/bin/drat2er tph8.cnf tph8.drat -v tph8.er
