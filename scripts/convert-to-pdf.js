const puppeteer = require('puppeteer');
const path = require('path');
const { exec } = require('child_process');

const [,, inputHtml, outputPdf] = process.argv;

if (!inputHtml || !outputPdf) {
  console.error('Usage: node convert-to-pdf.js <input.html> <output.pdf>');
  process.exit(1);
}

// Extract directory and filename
const serveDir = path.dirname(inputHtml);
const htmlFile = path.basename(inputHtml);

(async () => {
  // Start local HTTP server to serve HTML + images
  const port = 8082;
  const server = exec(`npx http-server ${serveDir} -p ${port}`);

  // Wait a bit to ensure server starts
  await new Promise(resolve => setTimeout(resolve, 1000));

  const browser = await puppeteer.launch({
    headless: 'new',
    args: ['--no-sandbox', '--disable-setuid-sandbox']
  });

  const page = await browser.newPage();
  const url = `http://localhost:${port}/${htmlFile}`;
  await page.goto(url, { waitUntil: 'networkidle0' });

  await page.pdf({
    path: outputPdf,
    format: 'A4',
    printBackground: true,
    margin: { top: '1cm', bottom: '1cm', left: '1cm', right: '1cm' }
  });

  await browser.close();
  server.kill(); // Stop the local server
})();

