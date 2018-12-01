/*
pxCore Copyright 2005-2018 John Robinson

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

px.import({scene:"px:scene.1.js"}).then( function ready(imports) {
var scene = imports.scene;
var inputRes = scene.create({t:"imageResource", id:"imageRes", url:px.getBaseFilePath()+"supportfiles/input2.png"});
function TextInput()
{

}

module.exports = TextInput;

}).catch(function importFailed(err){
    console.error("Import failed for importFile.js: " + err);
});
