function doGet(e) {
    var params = e.parameter;
    var data = params.data;

    /* Input your file ID here */
    var SpreadSheet = SpreadsheetApp.openByUrl('FILE_ID_HERE');

    /* Input your sheet name here */
    var Sheet = SpreadSheet.getSheetByName('SHEET_NAME_HERE');

    /*** START writing data to Google Sheet ***/
    var LastRow = Sheet.getLastRow();
    data = data.split(',');
    data.forEach(function(e,i){
        Sheet.getRange(LastRow+1, i+1).setValue(e);
    });
}