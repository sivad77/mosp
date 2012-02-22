#!/usr/bin/perl -w
use Time::HiRes qw(usleep ualarm gettimeofday tv_interval);
$args = shift @ARGV;

foreach $file (@ARGV) {
	($name = $file) =~ s/^.*\///; 
	print "$name ";
	
	#time execution
	@result = `./mosp $args $file`;
	
	# get rid of letters 
	s/^[a-zA-Z: ]*// foreach @result;
	
	# get rid of new lines
	chomp foreach @result;
	
	print "\t\t $result[2] \t $result[7] \t $result[8]  \n";	
}
