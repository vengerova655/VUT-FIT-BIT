<?php
# @author: Veronika Vengerova
# login: xvenge01
# date: 19.4.2022

include 'helper.php';

$a = Tester::get_instance();
$a->parse_arg($argv);

$test = new Testing($a);
$test->test();

ini_set('display_errors', 'stderr');


?>
<?php
$testNum = $test->all_files();
$backgrndColor = "lightred";
if ($testNum == 0){
    $x = 0;
}
else {
    $x = floor(($test->count_correct() / $test->all_files()) * 100);
    if ($x > 75)
        $backgrndColor = "lightgreen";
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Test</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>
<div class="outer">
    <div class="header">
        <div class="percento  <?php echo $backgrndColor; ?>">
            <p id="percent">Percentualna uspesnost: <?php
                echo $x; ?>
                %</p>
            <div id="border">
                <div class="green" style="height:1.8rem;
                        border: solid 0.1rem whitesmoke;width:<?php
                    echo $x;
                ?>%"></div>
            </div>
        </div>
        <div class="right">
            <p>Počet testov celkovo: <?php
                echo $testNum;
                ?></p>
            <p class="correct">Pocet spravnych: <?php
                echo $test->count_correct();
                ?></p>
            <p class="incorrect">Pocet nesprávnych: <?php
                echo $testNum - $test->count_correct();
                ?></p>
        </div>
    </div>
    <div id="tester">

        <?php
            foreach ($test->results->results as $key=>$result) {?>
                <div class="testResult <?php echo $result->get_corr() ?>border">
                    <div class="testNumber <?php echo $result->get_corr() ?>"><?php echo ($key+1) ?>.</div>
                    <div class="testName"><?php echo $result->get_path() ?></div>
                    <div class="result <?php echo $result->get_corr() ?>border  <?php echo $result->get_corr() ?>color"><?php echo $result->get_diff()?></div>
                    <div class="testOKNO <?php echo $result->get_corr() ?>"><?php echo $result->get_result_ok()?></div>
                </div>
        <?php
        }?>
    </div>
</div>
</body>
</html>