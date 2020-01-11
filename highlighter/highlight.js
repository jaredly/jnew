#!/usr/bin/env node

const Prism = require('prismjs');

const fs = require('fs');
const code = fs.readFileSync('/dev/stdin').toString();

const [_, __, language] = process.argv;

const html = Prism.highlight(code, Prism.languages.javascript,language || 'javascript');
console.log(html);

