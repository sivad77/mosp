#!/usr/bin/perl -w
use Time::HiRes qw(usleep ualarm gettimeofday tv_interval);

$testName = shift @ARGV;
$args = shift @ARGV;
print '\begin{table}[H]
\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|c|} \hline';
print "\n";

print 'Instance&Best&$|C|$&$|P|$&Optimal&Parts&Subs&Steps&Reused&Opening&Fail&Time(ms) \\\\';
print '\hline';
print "\n";
foreach $file (@ARGV) {
	($name = $file) =~ s/^.*\///; 
	
	#time execution
	$t0 = [gettimeofday];
	@tbl = `./mosp $args $file`;
	$t1 = [gettimeofday];
	
	#get rid of name
	shift @tbl;
	shift @tbl;

	# get rid of letters 
	s/^[a-zA-Z: ]*// foreach @tbl;
	
	# get rid of new lines
	chomp foreach @tbl;
	
	$time = 1000 *(tv_interval $t0, $t1);
	print "\$$name\$&$tbl[0]&$tbl[2]&$tbl[3]&$tbl[4]&$tbl[5]&$tbl[6]&$tbl[7]&$tbl[8]&$tbl[9]&$tbl[10]&$time\\\\ \n";	
}

print '\hline
\end{tabular}';
print "\n\\caption{ $testName, total time taken }\n";
print '\end{table}';
print "\n";
