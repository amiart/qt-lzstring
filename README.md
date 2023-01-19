
<h2>Qt6 (cmake) implementation of LZ-String, version 1.4.5.</h2>
<p>
Based on the LZ-String compression algorithm found here:<br/>
http://pieroxy.net/blog/pages/lz-string/index.html
</p>
<p>
WTFPL Licence<br/>
http://www.wtfpl.net/
</p>
Implemented functions:<br/>
<ul>
<li>compress() / decompress() - <b>YES</b></li>
<li>compressToUTF16() / decompressFromUTF16() - <b>YES</b></li>
<li>compressToBase64() / decompressFromBase64() - <b>YES</b></li>
<li>compressToUint8Array() / decompressFromUint8Array() - <b>NO</b></li>
<li>compressToEncodedURIComponent() / decompressFromEncodedURIComponent() - <b>NO</b></li>
</ul>

<p>
	This Qt6 port relies on Qt5-compat for classes QStringRef and QTextCodec.<br/>
	Future improvements should focus on removing this dependency.<br/>
	<br/></p>