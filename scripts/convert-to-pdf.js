const puppeteer = require('puppeteer'); // Make sure to install full 'puppeteer' (not 'puppeteer-core')
const path = require('path');
const { exec } = require('child_process');
const http = require('http');

const [,, inputHtml, outputPdf] = process.argv;

if (!inputHtml || !outputPdf) {
  console.error('❌ Usage: node convert-to-pdf.js <input.html> <output.pdf>');
  process.exit(1);
}

const serveDir = path.dirname(inputHtml);
const htmlFile = path.basename(inputHtml);
const port = 8082;
const serverUrl = `http://localhost:${port}/${htmlFile}`;

function waitForServerReady(url, timeout = 5000) {
  return new Promise((resolve, reject) => {
    const start = Date.now();

    const check = () => {
      http.get(url, res => {
        if (res.statusCode === 200) resolve();
        else retry();
      }).on('error', retry);
    };

    const retry = () => {
      if (Date.now() - start > timeout) return reject(new Error('❌ Server did not start in time'));
      setTimeout(check, 250);
    };

    check();
  });
}

(async () => {
  console.log(`📦 Serving '${serveDir}' via http-server on port ${port}`);
  const server = exec(`npx http-server ${serveDir} -p ${port} -a 127.0.0.1`);

  // Log http-server output
  server.stdout.on('data', (data) => {
    process.stdout.write(`[http-server] ${data}`);
  });

  server.stderr.on('data', (data) => {
    process.stderr.write(`[http-server:ERR] ${data}`);
  });

  try {
    console.log(`⏳ Waiting for server to be ready at ${serverUrl}...`);
    await waitForServerReady(serverUrl);
    console.log(`✅ Server ready. Launching Puppeteer...`);

    const browser = await puppeteer.launch({
      headless: 'new',
      args: ['--no-sandbox', '--disable-setuid-sandbox']
    });

    const page = await browser.newPage();
    await page.goto(serverUrl, { waitUntil: 'networkidle0' });
    console.log(`📄 Loaded ${serverUrl} successfully.`);

    await page.pdf({
      path: outputPdf,
      format: 'A4',
      printBackground: true,
      margin: { top: '1cm', bottom: '1cm', left: '1cm', right: '1cm' }
    });

    await browser.close();
    server.kill();

    console.log(`✅ PDF generated successfully at: ${outputPdf}`);
  } catch (err) {
    console.error(`❌ Error: ${err.message}`);
    server.kill();
    process.exit(1);
  }
})();

