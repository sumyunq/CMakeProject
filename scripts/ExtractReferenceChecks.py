"""Compile the actual Markdown answer snippets, so published answers cannot drift silently."""
from pathlib import Path
import re
import sys


def extract(root, output):
    catalog = __import__("json").loads((root / "docs/course-catalog.json").read_text(encoding="utf-8"))
    includes = {"#include <opencv2/core.hpp>", "#include <opencv2/imgproc.hpp>",
                "#include <stdexcept>", "#include <iostream>", "#include <limits>"}
    fragments = []
    for entry in catalog:
        text = (root / entry["solution"]).read_text(encoding="utf-8")
        blocks = re.findall(r"```cpp\s*\n(.*?)```", text, re.S)
        if not blocks:
            continue
        code = "\n".join(blocks)
        includes.update(re.findall(r"^#include[^\n]+", code, re.M))
        code = re.sub(r"^#include[^\n]+", "", code, flags=re.M)
        fragments.append(f"namespace answer{entry['id']} {{\n{code}\n}}")
    main = r'''
void require(bool condition) { if (!condition) { throw std::runtime_error("published-answer-contract"); } }
int main() {
  try {
    const auto fit = answer14::fitHalfResolution(); require(std::abs(fit[0]-400)<1e-8 && std::abs(fit[1]-160)<1e-8);
    require(std::abs(answer15::depthSigma(40,.5).value()-15)<1e-8 && !answer15::depthSigma(0,.5));
    require(std::abs(answer16::velocity(0,0,10,.1).value()-100)<1e-8 && !answer16::velocity(0,1,10,1));
    require(std::abs(answer17::correctGated(0,1,1,1).value()-.5)<1e-8 && !answer17::correctGated(0,1,100,1));
    require(std::abs(answer18::correctFlat(60,10,.5).value()-100)<1e-8 && !answer18::correctFlat(255,10,.5));
    require(std::abs(answer19::peakOffset(7.75,9.75,9.75).value()-.5)<1e-8 && !answer19::peakOffset(1,1,1));
    const auto metrics=answer20::classificationMetrics(8,2,4).value(); require(std::abs(metrics[2]-8.0/11)<1e-8 && !answer20::classificationMetrics(0,0,0));
    const auto large=std::numeric_limits<int>::max(); require(answer20::classificationMetrics(large,large,large).value()[0]==.5);
    require(answer21::standardize(5,3,2).value()==1 && !answer21::standardize(5,3,0));
    const auto rgb=answer22::normalizeRgb({0,127.5F,255}).value(); require(rgb[0]==1 && rgb[1]==0 && rgb[2]==-1 && !answer22::normalizeRgb({-1,0,0}));
    const auto box=answer23::unletterbox({80,200,240,320}).value(); require(std::abs(box[0]-100)<1e-8 && std::abs(box[3]-200)<1e-8 && !answer23::unletterbox({0,0,10,10}));
    cv::Mat parent(4,6,CV_8UC1,cv::Scalar(1)); auto roi=parent(cv::Rect(1,1,3,2)); roi.setTo(7);
    require(answer2::sumRows(roi)==42); bool rejected=false;
    try { static_cast<void>(answer2::sumRows(cv::Mat{})); } catch (const std::invalid_argument&) { rejected=true; } require(rejected);
    cv::Mat image(3,7,CV_8UC1,cv::Scalar(150)); require(cv::countNonZero(answer24::thresholdRows(image))==21);
    const int shape[]={2,2,2}; cv::Mat tensor(3,shape,CV_8UC1,cv::Scalar(1)); rejected=false;
    try { static_cast<void>(answer24::thresholdRows(tensor)); } catch (const std::invalid_argument&) { rejected=true; } require(rejected);
    cv::Mat floating(2,3,CV_32F,cv::Scalar(2)), kernel=cv::Mat::ones(3,3,CV_32F), expected;
    cv::filter2D(floating,expected,-1,kernel,cv::Point(-1,-1),0,cv::BORDER_CONSTANT);
    require(cv::norm(answer27::correlateZero(floating,kernel),expected,cv::NORM_INF)==0); rejected=false;
    try { static_cast<void>(answer27::correlateZero(floating,cv::Mat::ones(2,2,CV_32F))); } catch (const std::invalid_argument&) { rejected=true; } require(rejected);
    std::cout << "PASS: published reference functions normal and boundary cases\n";
    return 0;
  } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
'''
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text("\n".join(sorted(includes)) + "\n" + "\n".join(fragments) + main, encoding="utf-8")


if __name__ == "__main__":
    extract(Path(sys.argv[1]), Path(sys.argv[2]))
