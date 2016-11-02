var fs = require ('fs'),
    gulp = require('gulp');

gulp.task('default',()=>{
    gulp.watch('stats.json',()=>{
        var now = new Date();
        fs.createReadStream('stats.json').pipe(fs.createWriteStream('stats/'+now.getHours()+now.getMinutes()+now.getSeconds()+now.getDate()+now.getMonth()+now.getFullYear()+'.json'));
    });
});
