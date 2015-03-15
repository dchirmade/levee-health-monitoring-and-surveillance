#Brief getting started guide.

# Questionnaires #

  * How to compile and get start?
> > [1](1.md) Checkout the souce code by refering: http://code.google.com/p/levee-health-monitoring-and-surveillance/source/checkout <br>
<blockquote><a href='2.md'>2</a> Then install all prerequisite dependencies packages by running ‘INSTALL-PREREQUISITE.ubuntu’ in main trunk directory. <br>
<a href='3.md'>3</a> To compile levee-mini-rule-engine go to levee-mini-rule-engine and run ‘make’ <br>
</blockquote><ul><li>How to install core system and how to configure cronjobs?<br>
<blockquote><a href='1.md'>1</a> Automated script to configure with cron jobs could be found in <a href='http://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk#trunk%2Fcron-jobs'>http://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk#trunk%2Fcron-jobs</a> <br>
<a href='2.md'>2</a> To configure cronjob add following lines into /etc/crontab and do ‘/etc/init.d/cron restart’ <br>
<code>0-59/20 * * * * root /&lt;checkoutpath&gt;/sensors/levee-health-monitoring-and-surveillance/cron-jobs/refresh-noaa-water-feeds.bash</code> <br>
<code>0-59/15 * * * * root /&lt;checkoutpath&gt;/sensors/levee-health-monitoring-and-surveillance/cron-jobs/refresh-noaa-weather-feeds.bash</code><br>
<a href='3.md'>3</a> This codebase can be compiled and run with superuser permission directly. Currently no .rpm or .deb release package has been built as such. <br></blockquote></li></ul>

<ul><li>How to run standalone POC based on hardware sensors?<br>
<blockquote><a href='1.md'>1</a> Please refer getting started guide for Arduino board on ‘<a href='http://arduino.cc/’'>http://arduino.cc/’</a> <br>
<a href='2.md'>2</a> Burn source code located in <a href='http://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk#trunk%2Flevee-data-acquisition-system%2Fhardware-sensors'>http://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk#trunk%2Flevee-data-acquisition-system%2Fhardware-sensors</a> on Arduno board <br></blockquote></li></ul>

<ul><li>How to run core monitoring system?<br>
<blockquote><a href='1.md'>1</a> Since all module generate their own binaries one can run them separately according to need. <br>
<a href='2.md'>2</a> But you can run code monitoring engine by running ‘mini-rule-engine’. Of course please make sure if ‘configurations.conf’ is in place and well configured according to need. <br></blockquote></li></ul>

<ul><li>How to add/remove/suspend new rule in monitoring system?<br>
<blockquote><a href='1.md'>1</a> Rule structure is pretty easy to understand and looks like <br>
<blockquote>int ruleIndex;            // Rule index number <br>
bool isRuleActive;        // If rule is active or inactive <br>
string ruleName;          // Human readable rule name <br>
string ruleDescription;   // Rule description if any <br>
unsigned int ruleAction;  // Action attached with rule <br>
string rulePayload;       // Any payload params needed by rule <br>
int ruleDelay;            // Wait time in rule execution sequence <br>
int nextRuleIndex;        // Next rule to be executed <br>
</blockquote><a href='2.md'>2</a> To Add rule call AppendNewRule method with right parameters. <br> Sample method could be located in mini-rule-engine.cpp <br> Flag isRuleActive decides if rule need to be executed or skipped (logical deletion) <br>
<a href='3.md'>3</a> One must need to define action method to handle any actions upon rule. <a href='br.md'>br</a></blockquote></li></ul>

<ul><li>How to install system on small low cost mini embedded single board computer?<br>
<blockquote>Please refer <a href='http://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk/optional-low-cost-host-system/alix-CF-readme.txt'>http://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk/optional-low-cost-host-system/alix-CF-readme.txt</a> <br>
</blockquote></li><li>How to do log rotation if debug mode enabled?<br>
<blockquote>Which ever log file you’d like to rotate, rotate the same using <a href='http://linuxcommand.org/man_pages/logrotate8.html'>http://linuxcommand.org/man_pages/logrotate8.html</a> command. <br>
</blockquote></li><li>Under which license source code will be published?<br>
<blockquote>All source code will be published under GNU general public license. <br>
</blockquote></li><li>Where is the Initial project development time line and activity?<br>
<blockquote>It could be found here at <a href='http://www.google-melange.com/gsoc/project/google/gsoc2011/q1w2e3r4/12001'>http://www.google-melange.com/gsoc/project/google/gsoc2011/q1w2e3r4/12001</a> <br>
</blockquote></li><li>Is there any demo logs, alerts logs?<br>
<blockquote><a href='https://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk/documentations-presentation/sample-mini-rule-engine.log'>https://code.google.com/p/levee-health-monitoring-and-surveillance/source/browse/trunk/documentations-presentation/sample-mini-rule-engine.log</a><br>