import hashlib
import os
import re
import subprocess
import urllib.request


def sanitize_svg(svg_data_bytes):
    """清洗 SVG 内容，将不可解析的 ex/em 相对单位转换为绝对数值，防止 rsvg-convert 失败"""
    try:
        svg_text = svg_data_bytes.decode("utf-8", errors="ignore")

        def repl_width(match):
            val = float(match.group(1))
            unit = match.group(2)
            factor = 8.5 if unit == "ex" else 17.0  # 科学微调公式缩放系数
            return f'width="{val * factor:.2f}"'

        def repl_height(match):
            val = float(match.group(1))
            unit = match.group(2)
            factor = 8.5 if unit == "ex" else 17.0
            return f'height="{val * factor:.2f}"'

        # 替换 width 和 height 中的 ex/em 单位
        svg_text = re.sub(
            r'width=["\']([0-9.]+)(ex|em)["\']',
            svg_text_repl := repl_width,
            svg_text,
            count=1,
        )
        svg_text = re.sub(
            r'height=["\']([0-9.]+)(ex|em)["\']', repl_height, svg_text, count=1
        )

        return svg_text.encode("utf-8")
    except Exception as e:
        print(f"  ├─ ⚠️ SVG 清洗失败: {e}")
        return svg_data_bytes


def batch_md_to_pdf():
    # 1. 创建本地图片目录
    os.makedirs("media", exist_ok=True)

    headers = {
        "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
        "Referer": "https://www.yuque.com/",
    }

    # 2. 检索待处理的 .md 文件
    md_files = [
        f
        for f in os.listdir(".")
        if f.endswith(".md") and not f.endswith("_local.md")
    ]

    if not md_files:
        print("❌ 当前目录下未找到任何 .md 文件！")
        return

    print(
        f"📂 找到 {len(md_files)} 个待处理的 Markdown 文件，开始执行【全资产本地化流水线】...\n"
    )

    for md_file in md_files:
        base_name = os.path.splitext(md_file)[0]
        local_md = f"{base_name}_local.md"
        output_pdf = f"{base_name}.pdf"

        print(f"=========================================")
        print(f"🚀 正在处理: {md_file} -> {output_pdf}")
        print(f"=========================================")

        with open(md_file, "r", encoding="utf-8") as f:
            content = f.read()

        # 【核心改进】重新设计正则表达式。精准匹配所有属于语雀/nlark 的网络资源（包含公式与普通图片）
        # [^\s)\"'<>]+? 和 [^\s)\"'<>]* 确保严格闭合在 Markdown 括号或 HTML 引号边界内，吞掉所有类似 #card=math 或 ?x-oss... 的长尾巴！
        urls = re.findall(
            r"https://[^\s)\"'<>]+?(?:nlark\.com|yuque\.com)[^\s)\"'<>]*",
            content,
        )
        urls = list(set(urls))

        if urls:
            print(
                f"  ├─ 🔍 检测到 {len(urls)} 个语雀网络资产，开始执行防盗链本地化与类型清洗..."
            )
            for i, url in enumerate(urls):
                url_hash = hashlib.md5(url.encode("utf-8")).hexdigest()

                # 智能识别后缀
                lower_url = url.lower()
                if ".svg" in lower_url:
                    ext = ".svg"
                elif ".png" in lower_url:
                    ext = ".png"
                elif ".jpg" in lower_url or ".jpeg" in lower_url:
                    ext = ".jpg"
                elif ".gif" in lower_url:
                    ext = ".gif"
                else:
                    ext = ".png"  # 兜底后缀

                local_filename = f"media/asset_{url_hash}{ext}"
                final_replacement_path = local_filename

                # A. 伪装下载资产（如果是普通插图，到这里就完美结束并安全使用了）
                # 增加判断，如果已经是转好的 pdf 或者是存在的图片，不再重复下载
                if not os.path.exists(local_filename) and not (
                    ext == ".svg"
                    and os.path.exists(f"media/asset_{url_hash}.pdf")
                ):
                    try:
                        req = urllib.request.Request(url, headers=headers)
                        with urllib.request.urlopen(req) as response:
                            file_data = response.read()

                        # 如果是 SVG 公式，进行特殊的相对单位清洗
                        if ext == ".svg":
                            file_data = sanitize_svg(file_data)

                        with open(local_filename, "wb") as out_file:
                            out_file.write(file_data)
                    except Exception as e:
                        print(f"  ├─ ❌ 资产 [{i+1}] 下载失败: {e}")
                        continue

                # B. 如果是 SVG 公式，进一步调用 rsvg-convert 转换为 LaTeX 原生支持的矢量 PDF
                if ext == ".svg":
                    pdf_filename = f"media/asset_{url_hash}.pdf"
                    if not os.path.exists(pdf_filename):
                        try:
                            cmd = [
                                "rsvg-convert",
                                "-f",
                                "pdf",
                                "-o",
                                pdf_filename,
                                local_filename,
                            ]
                            res = subprocess.run(
                                cmd, stderr=subprocess.PIPE, text=True
                            )
                            if res.returncode != 0:
                                print(
                                    f"  ├─ ⚠️ 公式 [{i+1}] 矢量化失败，将使用原始 SVG 兜底"
                                )
                                final_replacement_path = local_filename
                            else:
                                final_replacement_path = pdf_filename
                        except Exception as e:
                            print(f"  ├─ ⚠️ 无法调用 rsvg-convert 核心: {e}")
                            final_replacement_path = local_filename
                    else:
                        final_replacement_path = pdf_filename

                # C. 核心一击：用无比纯净、毫无多余尾巴的本地相对路径，【完全替换】整串牛皮癣一样的网络 URL
                content = content.replace(url, final_replacement_path)

            print(f"  ├─ ✅ 所有资产（公式+插图）本地化与路径清洗全部完成！")
        else:
            print("  ├─ 💡 未检测到网络资产，跳过本地化步骤。")

        # 写入临时 md 文件
        with open(local_md, "w", encoding="utf-8") as f:
            f.write(content)

        # 3. 构造 Pandoc 命令参数
        # pandoc_cmd = [
        #     "pandoc",
        #     local_md,
        #     "-o",
        #     output_pdf,
        #     "--pdf-engine=xelatex",
        #     "--listings",
        #     "--toc",
        #     "--number-sections",
        #     "-H",
        #     "header.tex",
        #     "-V",
        #     "documentclass=extarticle",
        #     "-V",
        #     "classoption=onecolumn",
        #     "-V",
        #     "geometry:margin=1.0cm",
        #     "-V",
        #     "fontsize=9pt",
        #     "-V",
        #     "mainfont=Noto Sans CJK SC",
        #     "-V",
        #     "CJKmainfont=Noto Sans CJK SC",
        #     "-V",
        #     "CJKmonofont=Noto Sans Mono CJK SC",
        # ]
        pandoc_cmd = [
            "pandoc",
            local_md,
            "-f",                            # 新增：指定输入格式
            "markdown-implicit_figures",     # 新增：禁用隐式图表自动编号
            "-o",
            output_pdf,
            "--pdf-engine=xelatex",
            "--listings",
            "--toc",
            "--number-sections",
            "--from=markdown+tex_math_single_backslash",
            "-H",
            "header.tex",
            "-V",
            "documentclass=extarticle",
            "-V",
            "classoption=onecolumn",
            # "-V",
            # "geometry:margin=1.0cm",
            "-V",
            "geometry:top=1.0cm,bottom=1.4cm,left=1.0cm,right=1.0cm",  # 下边距稍微留大一点点到 1.4cm
            "-V",
            "geometry:footskip=0.5cm",  # 核心：强行把页码往上抬，让它紧贴正文下方 0.5cm 处


            "-V",
            "fontsize=9pt",
            "-V",
            "mainfont=Noto Sans CJK SC",
            "-V",
            "CJKmainfont=Noto Sans CJK SC",
            "-V",
            "CJKmonofont=Noto Sans Mono CJK SC",
        ]
        # 4. 执行 Pandoc 编译
        try:
            print("  ├─ 🛠️  正在调用 Pandoc 编译成双栏 PDF...")
            subprocess.run(
                pandoc_cmd, check=True, stderr=subprocess.PIPE, text=True
            )
            print(f"  └─ 🎉 编译成功！已生成: {output_pdf}")
        except subprocess.CalledProcessError as e:
            print(f"  └─ ❌ Pandoc 编译失败！")
            print(f"      错误信息:\n{e.stderr}")
        finally:
            # 5. 清理中间临时文件
            if os.path.exists(local_md):
                os.remove(local_md)
        print()

    print("🎉 所有文件处理完毕！")


if __name__ == "__main__":
    batch_md_to_pdf()
