"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.killTestRun = exports.runAcutests = exports.getTestBinary = exports.loadAcutestTests = exports.CAcutestTestInfo = exports.CAcutestSuiteInfo = void 0;
const tslib_1 = require("tslib");
const vscode = require("vscode");
const fs = require("fs");
const child_process_1 = require("child_process");
const pathNJS = require('path');
const processes = Array();
let testMapping = {};
class CAcutestSuiteInfo {
    constructor(inputString) {
        this.type = "suite";
        this.id = inputString;
        this.label = inputString;
        this.children = new Array();
    }
}
exports.CAcutestSuiteInfo = CAcutestSuiteInfo;
class CAcutestTestInfo {
    constructor(testString) {
        this.type = "test";
        this.id = testString;
        this.label = testString;
    }
}
exports.CAcutestTestInfo = CAcutestTestInfo;
let AcutestSuite;
function loadAcutestTests() {
    var workspaceRoot = (vscode.workspace.workspaceFolders || [])[0].uri.fsPath;
    var testGroups = vscode.workspace.getConfiguration('acutestExplorer').testGroups;
    var exeFile = vscode.workspace.getConfiguration('acutestExplorer').testGroupsExeFile || 'test.out';
    var srcFile = vscode.workspace.getConfiguration('acutestExplorer').testGroupsSrcFile || 'test_runner.c';
    if (workspaceRoot == null) {
        return Promise.resolve(AcutestSuite);
    }

    if (Array.isArray(testGroups) && testGroups.length > 0) {
        AcutestSuite = new CAcutestSuiteInfo('Acutest Suite');
        testMapping = {};

        for (const groupEntry of testGroups) {
            if (!groupEntry)
                continue;
            const groupDir = pathNJS.isAbsolute(groupEntry)
                ? groupEntry
                : pathNJS.join(workspaceRoot, groupEntry);

            const groupName = pathNJS.basename(groupDir);
            const exePath = pathNJS.join(groupDir, exeFile);
            const srcPath = pathNJS.join(groupDir, srcFile);
            if (!fs.existsSync(exePath)) {
                continue;
            }

            let listOutput = '';
            try {
                listOutput = child_process_1.execFileSync(exePath, ['--list'], {
                    cwd: groupDir,
                    encoding: 'utf8'
                });
            }
            catch (err) {
                continue;
            }

            const lines = listOutput
                .split(/\r?\n/)
                .map((line) => line.trim())
                .filter((line) => line.length > 0 && !/^Unit tests:?$/i.test(line));

            if (lines.length === 0) {
                continue;
            }

            const groupSuite = new CAcutestSuiteInfo(groupName);
            AcutestSuite.children.push(groupSuite);

            for (const line of lines) {
                const testName = line;
                const testId = groupName + '::' + testName;
                groupSuite.children.push({
                    type: 'test',
                    id: testId,
                    label: testName,
                    skipped: false
                });
                testMapping[testId] = {
                    exe: exePath,
                    testName: testName,
                    src: srcPath,
                    group: groupName
                };
            }
        }

        return Promise.resolve(AcutestSuite);
    }

    var srcfile = vscode.workspace.getConfiguration('acutestExplorer').testsourceFile || vscode.workspace.getConfiguration('acutestExplorer').testSourceFile;
    if (workspaceRoot == null || srcfile == '') {
        return Promise.resolve(AcutestSuite);
    }

    AcutestSuite = new CAcutestSuiteInfo('Acutest Suite');
    const subsuite = new CAcutestSuiteInfo(srcfile);
    AcutestSuite.children.push(subsuite);
    var srcPath = pathNJS.isAbsolute(srcfile) ? srcfile : pathNJS.join(workspaceRoot, srcfile);
    var data = '';
    try {
        data = fs.readFileSync(srcPath, 'utf8');
    }
    catch (err) {
        return Promise.resolve(AcutestSuite);
    }
    var index = data.indexOf('TEST_LIST');
    if (index === -1) {
        return Promise.resolve(AcutestSuite);
    }
    var start = data.indexOf('{', index);
    var end = data.indexOf('};', index);
    if (start === -1 || end === -1 || end <= start) {
        return Promise.resolve(AcutestSuite);
    }
    var test_list = data.slice(start, end);
    index = 0;
    while ((index = test_list.indexOf('"', index + 1)) != -1) {
        var next = test_list.indexOf('"', index + 1);
        if (next === -1)
            break;
        var name = test_list.slice(index + 1, next);
        subsuite.children.push({
            type: 'test',
            id: name,
            label: name,
            skipped: false
        });
        index = next + 1;
    }
    return Promise.resolve(AcutestSuite);
}
exports.loadAcutestTests = loadAcutestTests;
function getTestBinary() {
    var workspaceRoot = (vscode.workspace.workspaceFolders || [])[0].uri.fsPath;
    var exePath = vscode.workspace.getConfiguration("acutestExplorer").testExecutable;
    return pathNJS.isAbsolute(exePath) ? exePath : pathNJS.join(workspaceRoot, exePath);
}
exports.getTestBinary = getTestBinary;
function runAcutests(tests, testStatesEmitter) {
    return tslib_1.__awaiter(this, void 0, void 0, function* () {
        for (const suiteOrTestId of tests) {
            const node = findNode(AcutestSuite, suiteOrTestId);
            if (node) {
                yield runNode(node, testStatesEmitter);
            }
        }
    });
}
exports.runAcutests = runAcutests;
function findNode(searchNode, id) {
    if (searchNode.id === id) {
        return searchNode;
    }
    else if (searchNode.type === 'suite') {
        for (const child of searchNode.children) {
            const found = findNode(child, id);
            if (found)
                return found;
        }
    }
    return undefined;
}
function killTestRun() {
    processes.forEach(p => p.kill("SIGTERM"));
}
exports.killTestRun = killTestRun;
function runNode(node, testStatesEmitter) {
    return tslib_1.__awaiter(this, void 0, void 0, function* () {
        if (node.type === 'suite') {
            testStatesEmitter.fire({ type: 'suite', suite: node.id, state: 'running' });
            for (const child of node.children) {
                yield runNode(child, testStatesEmitter);
            }
            testStatesEmitter.fire({ type: 'suite', suite: node.id, state: 'completed' });
        }
        else {
            testStatesEmitter.fire({ type: 'test', test: node.id, state: 'running' });
            const mapping = testMapping[node.id];
            const binaryPath = mapping ? mapping.exe : getTestBinary();
            const actualTestName = mapping ? mapping.testName : node.id;
            const event = yield runTestBinary(binaryPath, node.id, actualTestName);
            testStatesEmitter.fire(event);
        }
    });
}
function runTestBinary(path, testId, testName) {
    return tslib_1.__awaiter(this, void 0, void 0, function* () {
        const promise = new Promise((resolve, reject) => {
            const runProcess = child_process_1.execFile(path, [testName], { cwd: path.slice(0, path.lastIndexOf(pathNJS.sep)) }, (error, stdout, stderr) => {
                let state = undefined;
                let message = stdout.trim();
                if (error && error.code === null) {
                    state = 'errored';
                    message = stderr || stdout.trim();
                }
                const regexPattern = /Test .+?\.{3}\s+((\[ OK \])|\[ FAILED \])/gs;
                const result = regexPattern.exec(stdout);
                if (result != null) {
                    var passed = (result[0].indexOf("[ OK ]") > 0);
                    state = passed ? "passed" : "failed";
                    message = stdout.replace(result[0], '').trim();
                }
                else if (stdout.indexOf("[ OK ]") !== -1 || stdout.indexOf("[ FAILED ]") !== -1) {
                    state = stdout.indexOf("[ OK ]") !== -1 ? "passed" : "failed";
                }
                if (state == null) {
                    state = error ? 'errored' : 'failed';
                    message = stderr || stdout.trim();
                }
                const event = {
                    type: 'test',
                    test: testId,
                    state: state,
                    message: message,
                    decorations: undefined
                };
                resolve(event);
            });
            processes.push(runProcess);
        });
        return Promise.resolve(promise);
    });
}
//# sourceMappingURL=Acutest.js.map