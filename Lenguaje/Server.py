#Servidor local es decir host local



#!/usr/bin/env python3
"""
Servidor local ligero para IoT Animal Monitor
Permite servir los archivos estáticos en http://localhost:8000
"""

import http.server
import socketserver
import os
import webbrowser
import sys

PORT = 8000
DIRECTORY = os.path.dirname(os.path.abspath(__file__))

class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)

    def end_headers(self):
        # Disable caching for development
        self.send_header('Cache-Control', 'no-store, no-cache, must-revalidate')
        super().end_headers()

def main():
    os.chdir(DIRECTORY)
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        print("=" * 60)
        print("   IoT ANIMAL MONITOR - SERVIDOR LOCAL ACTIVO")
        print(f"  URL Local: http://localhost:{PORT}")
        print("  Presiona Ctrl+C para detener el servidor")
        print("=" * 60)
        
        # Open browser automatically if not in headless mode
        if len(sys.argv) > 1 and sys.argv[1] == '--open':
            webbrowser.open(f"http://localhost:{PORT}")
            
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServidor detenido.")

if __name__ == "__main__":
    main()

