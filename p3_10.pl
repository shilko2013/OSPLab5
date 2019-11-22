#!/usr/bin/perl
use strict;
use warnings FATAL => 'all';

$ENV{PATH}="/usr/bin";
$ENV{CDPATH}="";
$ENV{ENV}="";

die "usage: p3_10.pl filename\n" unless @ARGV;
open my $file, "<:", $ARGV[0] or die $!;
open my $wc, "| wc" or die $!;

$/ = undef;
exit 0 unless defined($_ = <$file>);
print $wc grep {$/=!$/} split //;
