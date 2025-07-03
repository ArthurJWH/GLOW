# -*- mode: python ; coding: utf-8 -*-

a = Analysis(
    ['src\\main.py'],
    pathex=['src'],
    binaries=[],
    datas=[
        ('assets\\img\\', 'assets\\img'),
        ('assets\\ML_models\\', 'assets\\ML_models'),
        ('docs\\', 'docs')
    ],
    hiddenimports=['cv2', 'sklearn', 'sklearn.pipeline', 'sklearn.neural_network'],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=None,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=None)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name='G.L.O.W.',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=['assets\\img\\icon.png'],
    exclude_binaries=True,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='glow'
)
