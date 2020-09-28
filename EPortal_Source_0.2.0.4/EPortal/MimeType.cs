using System;
using System.Collections.Generic;
using System.IO;

public static class MimeType
{
    private static readonly IDictionary<string, string> mappings =
        new Dictionary<string, string>(StringComparer.InvariantCultureIgnoreCase)
        {
            { "ai",    "application/postscript" },
            { "art",   "image/x-jg" },
            { "bmp",   "image/bmp" },
            { "chm",   "application/octet-stream" },
            { "clp",   "application/x-msclip" },
            { "cmx",   "image/x-cmx" },
            { "dcr",   "image/x-kodak-dcr" },
            { "dib",   "image/bmp" },
            { "doc",   "application/msword" },
            { "docm",  "application/vnd.ms-word.document.macroEnabled.12" },  // security risk?
            { "docx",  "application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
            { "dvi",   "application/x-dvi" },
            { "emz",   "application/octet-stream" },
            { "eps",   "application/postscript" },
            { "etx",   "text/x-setext" },
            { "evy",   "application/envoy" },
            { "fif",   "application/fractals" },
            { "gif",   "image/gif" },
            { "hdml",  "text/x-hdml" },
            { "htc",   "text/x-component" },
            { "htm",   "text/html" },
            { "html",  "text/html" },
            { "ief",   "image/ief" },
            { "jfif",  "image/pjpeg" },
            { "jpe",   "image/jpeg" },
            { "jpeg",  "image/jpeg" },
            { "jpg",   "image/jpeg" },
            { "latex", "application/x-latex" },
            { "lit",   "application/x-ms-reader" },
            { "m13",   "application/x-msmediaview" },
            { "m14",   "application/x-msmediaview" },
            { "mac",   "image/x-macpaint" },
            { "man",   "application/x-troff-man" },
            { "me",    "application/x-troff-me" },
            { "mht",   "message/rfc822" },
            { "mhtml", "message/rfc822" },
            { "mix",   "image/vnd.mix" },
            { "ms",    "application/x-troff-ms" },
            { "nc",    "application/x-netcdf" },
            { "odg",   "application/vnd.oasis.opendocument.graphics" },
            { "odi",   "application/vnd.oasis.opendocument.image" },
            { "odm",   "application/vnd.oasis.opendocument.text-master" },
            { "odt",   "application/vnd.oasis.opendocument.text" },
            { "oth",   "application/vnd.oasis.opendocument.text-web" },
            { "pbm",   "image/x-portable-bitmap" },
            { "pct",   "image/pict" },
            { "pcx",   "image/x-pcx" },
            { "pcz",   "application/octet-stream" },
            { "pdf",   "application/pdf" },
            { "pgm",   "image/x-portable-graymap"},
            { "pic",   "image/pict" },
            { "pict",  "image/pict" },
            { "png",   "image/png" },
            { "pnm",   "image/x-portable-anymap" },
            { "pnt",   "image/x-macpaint" },
            { "pntg",  "image/x-macpaint" },
            { "pnz",   "image/png" },
            { "ppm",   "image/x-portable-pixmap" },
            { "ps",    "application/postscript"  },
            { "psd",   "image/psd" },
            { "qht",   "text/x-html-insertion" },
            { "qhtm",  "text/x-html-insertion" },
            { "qti",   "image/x-quicktime" },
            { "qtif",  "image/x-quicktime" },
            { "qxd",   "application/quarkxpress" },
            { "ras",   "image/x-cmu-raster" },
            { "rgb",   "image/x-rgb" },
            { "roff",  "application/x-troff" },
            { "rtf",   "application/rtf" },
            { "rtx",   "text/richtext" },
            { "sgml",  "text/sgml" },
            { "svg",   "image/svg+xml" },
            { "t",     "application/x-troff" },
            { "tex",   "application/x-tex" },
            { "texi",  "application/x-texinfo" },
            { "tif",   "image/tiff" },
            { "tiff",  "image/tiff" },
            { "tr",    "application/x-troff" },
            { "txt",   "text/plain" },
            { "vml",   "text/xml" },
            { "vsd",   "application/vnd.visio" },
            { "vst",   "application/vnd.visio" },
            { "vsw",   "application/vnd.visio" },
            { "wbmp",  "image/vnd.wap.wbmp" },
            { "wdp",   "image/vnd.ms-photo" },
            { "webp",  "image/webp" },
            { "wmf",   "application/x-msmetafile" },
            { "wps",   "application/vnd.ms-works" },
            { "wri",   "application/x-mswrite" },
            { "wrl",   "x-world/x-vrml" },
            { "wrz",   "x-world/x-vrml" },
            { "xbm",   "image/x-xbitmap" },
            { "xht",   "application/xhtml+xml" },
            { "xhtml", "application/xhtml+xml" },
            { "xpm",   "image/x-xpixmap" },
            { "xps",   "application/vnd.ms-xpsdocument" },
            { "xwd",   "image/x-xwindowdump" },
        };

    public static string Get(string name)
    {
        string extension = Path.GetExtension(name);
        string mime = null;

        if (!String.IsNullOrEmpty(extension))
            mappings.TryGetValue(extension.Substring(1), out mime);

        return mime;
    }

    public static bool Ambiguous(string name)
    {
        string extension = Path.GetExtension(name).ToLower();

        return extension == ".doc" || extension == ".docx";
    }

    public static string Reget(byte[] content)
    {
        string realExtension = "doc";
        string mime = null;

        if (content.Length >= 2)
        {
            // all characters are ASCII
            if (content[0] == 'P' && content[1] == 'K')
                realExtension = "docx";
            else if (content[0] == '<' && (content[1] == 'h' || content[1] == 'H'))
                realExtension = "html";
        }

        mappings.TryGetValue(realExtension, out mime);
        return mime;
    }
}
